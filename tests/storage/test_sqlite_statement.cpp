#include <boost/test/unit_test.hpp>

#include "test_fixture.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

BOOST_AUTO_TEST_SUITE(SqliteStatementTests)

BOOST_FIXTURE_TEST_CASE(test_bind_and_execute_insert, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name, age, salary) VALUES (@name, @age, @salary)"
    );

    stmt->bindString("@name", "John Doe");
    stmt->bindInt64("@age", 30);
    stmt->bindDouble("@salary", 50000.50);

    int64_t rows = stmt->execute();
    BOOST_CHECK_EQUAL(rows, 1);

    auto rs = db.query("SELECT name, age, salary FROM statement_test WHERE name = 'John Doe'");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), "John Doe");
    BOOST_CHECK_EQUAL(rs->getInt64(1), 30);
    BOOST_CHECK_CLOSE(rs->getDouble(2), 50000.50, 0.001);
}

BOOST_FIXTURE_TEST_CASE(test_bind_null, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name, age) VALUES (@name, @age)"
    );

    stmt->bindString("@name", "Null Age");
    stmt->bindNull("@age");
    stmt->execute();

    auto rs = db.query("SELECT age FROM statement_test WHERE name = 'Null Age'");
    rs->next();
    BOOST_CHECK(rs->isNull(0));
}

BOOST_FIXTURE_TEST_CASE(test_bind_blob, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name, data) VALUES (@name, @data)"
    );

    Blob testData = { 0x01, 0x02, 0x03, 0xFF, 0x00 };
    stmt->bindString("@name", "Blob Test");
    stmt->bindBlob("@data", testData);
    stmt->execute();

    auto rs = db.query("SELECT data FROM statement_test WHERE name = 'Blob Test'");
    rs->next();
    Blob retrieved = rs->getBlob(0);

    BOOST_CHECK_EQUAL_COLLECTIONS(testData.begin(), testData.end(), retrieved.begin(), retrieved.end());
}

BOOST_FIXTURE_TEST_CASE(test_bind_datetime, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name, created) VALUES (@name, @created)"
    );

    auto now = std::chrono::system_clock::now();
    stmt->bindString("@name", "DateTime Test");
    stmt->bindDateTime("@created", now);
    stmt->execute();

    auto rs = db.query("SELECT created FROM statement_test WHERE name = 'DateTime Test'");
    rs->next();
    DateTime retrieved = rs->getDateTime(0);

    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - retrieved).count();
    BOOST_CHECK(std::abs(diff) <= 1);
}

BOOST_FIXTURE_TEST_CASE(test_multiple_bind_and_reset, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name, age) VALUES (@name, @age)"
    );

    stmt->bindString("@name", "User1");
    stmt->bindInt64("@age", 25);
    stmt->execute();

    stmt->reset();
    stmt->bindString("@name", "User2");
    stmt->bindInt64("@age", 30);
    stmt->execute();

    auto rs = db.query("SELECT name, age FROM statement_test WHERE name IN ('User1', 'User2') ORDER BY name");

    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), "User1");
    BOOST_CHECK_EQUAL(rs->getInt64(1), 25);

    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), "User2");
    BOOST_CHECK_EQUAL(rs->getInt64(1), 30);
}

BOOST_FIXTURE_TEST_CASE(test_execute_query_with_statement, StatementTestFixture)
{
    // Полная очистка таблицы перед тестом
    db.execute("DROP TABLE IF EXISTS statement_test");
    db.execute(
        "CREATE TABLE IF NOT EXISTS statement_test ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "age INTEGER, "
        "salary REAL, "
        "data BLOB, "
        "created DATETIME)"
    );

    auto conn = db.getConnection();

    // Вставляем данные заново
    for (int i = 1; i <= 5; ++i)
    {
        auto insertStmt = conn->prepareStatement(
            "INSERT INTO statement_test (name, age) VALUES (@name, @age)"
        );
        std::string name = "User" + std::to_string(i);
        int age = 20 + i; // 21, 22, 23, 24, 25
        insertStmt->bindString("@name", name);
        insertStmt->bindInt64("@age", age);
        int64_t rows = insertStmt->execute();
        BOOST_CHECK_EQUAL(rows, 1);
    }

    // Проверяем все вставленные записи
    auto allRs = db.query("SELECT name, age FROM statement_test ORDER BY age");
    std::cout << "All records in table:" << std::endl;
    while (allRs->next())
    {
        std::cout << "  " << allRs->getString(0) << " -> age " << allRs->getInt64(1) << std::endl;
    }

    // Проверяем типы данных в таблице
    auto pragmaRs = db.query("PRAGMA table_info(statement_test)");
    std::cout << "Table schema:" << std::endl;
    while (pragmaRs->next())
    {
        std::cout
            << "  Column " << pragmaRs->getString(1)
            << " type: " << pragmaRs->getString(2) << std::endl;
    }

    // Прямой запрос без подготовленного statement для сравнения
    auto directRs = db.query("SELECT name, age FROM statement_test WHERE age > 22 ORDER BY age");
    std::cout << "Direct query results (age > 22):" << std::endl;
    while (directRs->next())
    {
        std::cout << "  " << directRs->getString(0) << " -> age " << directRs->getInt64(1) << std::endl;
    }

    // Теперь тестируем подготовленный statement
    auto queryStmt = conn->prepareStatement(
        "SELECT name, age FROM statement_test WHERE age > @min_age ORDER BY age"
    );

    // Пробуем bindInt64 с условием age > 22
    std::cout << "\nTesting with bindInt64 (age > 22):" << std::endl;
    queryStmt->bindInt64("@min_age", 22);

    auto rs = queryStmt->executeQuery();
    int count = 0;
    while (rs->next())
    {
        int age = rs->getInt64(1);
        std::string name = rs->getString(0);
        std::cout << "  Result " << count << ": " << name << " with age: " << age << std::endl;
        count++;
    }
    std::cout << "Total results with bindInt64(22): " << count << std::endl;

    // Ожидаем 3 результата для age > 22 (User3 age 23, User4 age 24, User5 age 25)
    BOOST_CHECK_EQUAL(count, 3);

    // Сбрасываем и пробуем bind с другим значением
    queryStmt->reset();
    std::cout << "\nTesting with bindInt64 (age > 20):" << std::endl;
    queryStmt->bindInt64("@min_age", 20);
    rs = queryStmt->executeQuery();
    count = 0;
    while (rs->next())
    {
        int age = rs->getInt64(1);
        std::string name = rs->getString(0);
        std::cout << "  Result " << count << ": " << name << " with age: " << age << std::endl;
        count++;
    }
    std::cout << "Total results with bindInt64(20): " << count << std::endl;

    BOOST_CHECK_EQUAL(count, 5);
}

BOOST_FIXTURE_TEST_CASE(test_invalid_parameter_name, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO statement_test (name) VALUES (@valid_name)"
    );

    BOOST_CHECK_THROW(stmt->bindString("@invalid_name", "test"), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_execute_non_insert_statement, StatementTestFixture)
{
    clearTable();

    auto conn = db.getConnection();

    conn->execute("INSERT INTO statement_test (name, age) VALUES ('Update Test', 10)");

    auto stmt = conn->prepareStatement(
        "UPDATE statement_test SET age = @new_age WHERE name = @name"
    );
    stmt->bindInt64("@new_age", 20);
    stmt->bindString("@name", "Update Test");

    int64_t rows = stmt->execute();
    BOOST_CHECK_EQUAL(rows, 1);

    auto rs = db.query("SELECT age FROM statement_test WHERE name = 'Update Test'");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 20);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
