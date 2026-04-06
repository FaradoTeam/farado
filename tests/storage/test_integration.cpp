#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include <boost/test/unit_test.hpp>

#include "vault-server/src/storage/database_factory.h"

namespace db::test
{

BOOST_AUTO_TEST_SUITE(IntegrationTests)

BOOST_AUTO_TEST_CASE(test_complete_workflow)
{
    auto db = DatabaseFactory::create(DatabaseType::Sqlite);

    DatabaseConfig config;
    config["database"] = "./test_data/integration_test.db";
    db->initialize(config);

    // Create schema
    db->execute("DROP TABLE IF EXISTS products");
    db->execute(R"(
        CREATE TABLE products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            price REAL,
            stock INTEGER,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )");

    // Insert data using prepared statements
    auto conn = db->getConnection();
    auto insertStmt = conn->prepareStatement(
        "INSERT INTO products (name, price, stock) VALUES (@name, @price, @stock)"
    );

    std::vector<std::tuple<std::string, double, int>> products = {
        { "Laptop", 999.99, 10 },
        { "Mouse", 25.50, 50 },
        { "Keyboard", 75.00, 30 }
    };

    for (const auto& [name, price, stock] : products)
    {
        insertStmt->reset();
        insertStmt->bindString("@name", name);
        insertStmt->bindDouble("@price", price);
        insertStmt->bindInt64("@stock", stock);
        int64_t result = insertStmt->execute();
        std::cout << "Inserted " << name << ", rows affected: " << result << std::endl;
    }

    // Проверяем все вставленные данные
    auto checkRs = db->query("SELECT name, price, stock FROM products ORDER BY price");
    std::cout << "All products in database:" << std::endl;
    while (checkRs->next())
    {
        std::cout
            << "  " << checkRs->getString(0) << " - price: " << checkRs->getDouble(1)
            << ", stock: " << checkRs->getInt64(2) << std::endl;
    }

    // Query with filter - only products with price > 50
    auto queryStmt = conn->prepareStatement(
        "SELECT name, price, stock FROM products WHERE price > @min_price ORDER BY price"
    );
    queryStmt->bindDouble("@min_price", 50.0);

    auto rs = queryStmt->executeQuery();

    // Ожидаем: Keyboard (75.00), Laptop (999.99) - отсортировано по возрастанию цены
    std::vector<std::string> expectedNames = { "Keyboard", "Laptop" };
    std::vector<double> expectedPrices = { 75.00, 999.99 };

    int idx = 0;
    while (rs->next())
    {
        std::string name = rs->getString("name");
        double price = rs->getDouble("price");

        std::cout << "Query result " << idx << ": " << name << " @ $" << price << std::endl;

        BOOST_CHECK_EQUAL(name, expectedNames[idx]);
        BOOST_CHECK_CLOSE(price, expectedPrices[idx], 0.01);
        idx++;
    }
    BOOST_CHECK_EQUAL(idx, 2);

    // Update with transaction
    db->transaction([&]()
                    {
        conn->execute("UPDATE products SET stock = stock - 1 WHERE name = 'Laptop'");
        conn->execute("UPDATE products SET stock = stock - 5 WHERE name = 'Mouse'"); });

    // Verify update
    rs = db->query("SELECT stock FROM products WHERE name = 'Laptop'");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 9);

    rs = db->query("SELECT stock FROM products WHERE name = 'Mouse'");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 45);

    // Delete with prepared statement
    auto deleteStmt = conn->prepareStatement("DELETE FROM products WHERE stock = @stock");
    deleteStmt->bindInt64("@stock", 0);
    int64_t deleted = deleteStmt->execute();
    BOOST_CHECK_EQUAL(deleted, 0);

    // Final count
    rs = db->query("SELECT COUNT(*) FROM products");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 3);

    db->shutdown();
}

BOOST_AUTO_TEST_CASE(test_concurrent_operations)
{
    auto db = DatabaseFactory::create(DatabaseType::Sqlite);

    DatabaseConfig config;
    config["database"] = "./test_data/concurrent_test.db";
    db->initialize(config);

    db->execute("DROP TABLE IF EXISTS counter");
    db->execute("CREATE TABLE counter (id INTEGER PRIMARY KEY, value INTEGER)");
    db->execute("INSERT INTO counter VALUES (1, 0)");

    // Используем WAL режим для лучшей конкурентности
    db->execute("PRAGMA journal_mode=WAL");
    db->execute("PRAGMA synchronous=NORMAL");

    std::vector<std::thread> threads;
    std::atomic<int> successes { 0 };
    std::mutex coutMutex;

    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(
            [&]()
            {
            try {
                // Используем retry логику для SQLite BUSY ошибок
                int retries = 3;
                while (retries-- > 0)
                {
                    try
                    {
                        // Получаем отдельное соединение для каждого потока
                        auto conn = db->getConnection();
                        
                        // Используем атомарный UPDATE вместо SELECT + UPDATE
                        // Это позволяет избежать вложенных транзакций
                        int64_t result = conn->execute(
                            "UPDATE counter SET value = value + 1 WHERE id = 1"
                        );
                        
                        if (result > 0) {
                            successes++;
                        }
                        break;
                    }
                    catch (const std::exception& e)
                    {
                        if (retries == 0
                            || (std::string(e.what()).find("locked") == std::string::npos &&
                                std::string(e.what()).find("busy") == std::string::npos))
                        {
                            throw;
                        }
                        // Ждём перед повторной попыткой
                        std::this_thread::sleep_for(std::chrono::milliseconds(10 * (3 - retries)));
                    }
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Concurrent operation failed: " << e.what() << std::endl;
            } }
        );
    }

    for (auto& t : threads)
    {
        t.join();
    }

    // Проверяем результат
    auto rs = db->query("SELECT value FROM counter WHERE id = 1");
    rs->next();
    int64_t finalValue = rs->getInt64(0);
    std::cout << "Final counter value: " << finalValue << ", successes: " << successes << std::endl;

    // Все операции должны быть успешными
    BOOST_CHECK_EQUAL(finalValue, 5);
    BOOST_CHECK_EQUAL(successes, 5);

    db->shutdown();
}

BOOST_AUTO_TEST_CASE(test_error_handling_and_recovery)
{
    auto db = DatabaseFactory::create(DatabaseType::Sqlite);

    DatabaseConfig config;
    config["database"] = "./test_data/error_test.db";
    db->initialize(config);

    db->execute("DROP TABLE IF EXISTS error_test");
    db->execute("CREATE TABLE error_test (id INTEGER PRIMARY KEY, data TEXT UNIQUE)");
    db->execute("INSERT INTO error_test VALUES (1, 'unique_value')");

    // Try to insert duplicate unique value
    BOOST_CHECK_THROW(
        db->execute("INSERT INTO error_test VALUES (2, 'unique_value')"),
        std::runtime_error
    );

    // Database should still be usable after error
    BOOST_CHECK_NO_THROW(db->execute("INSERT INTO error_test VALUES (2, 'another_value')"));

    // Verify data
    auto rs = db->query("SELECT COUNT(*) FROM error_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 2);

    db->shutdown();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
