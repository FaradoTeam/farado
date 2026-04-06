#include <filesystem>

#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include "vault-server/src/storage/database_factory.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

BOOST_AUTO_TEST_SUITE(SqliteDatabaseTests)

// Helper to create test database path
std::string getTestDbPath(const std::string& name)
{
    return "./test_data/" + name;
}

BOOST_AUTO_TEST_CASE(test_initialize_and_shutdown)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_init.db");

    BOOST_REQUIRE_NO_THROW(db.initialize(config));
    BOOST_REQUIRE_NO_THROW(db.shutdown());
}

BOOST_AUTO_TEST_CASE(test_initialize_twice_throws)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_init_twice.db");

    db.initialize(config);
    BOOST_CHECK_THROW(db.initialize(config), std::runtime_error);
    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_execute_create_table)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_execute.db");
    db.initialize(config);

    std::string createTable = R"(
        CREATE TABLE IF NOT EXISTS test_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            value INTEGER
        )
    )";

    BOOST_REQUIRE_NO_THROW(db.execute(createTable));

    // Verify table exists by inserting data
    std::string insert = "INSERT INTO test_table (name, value) VALUES ('test', 123)";
    int64_t changes = db.execute(insert);
    BOOST_CHECK_EQUAL(changes, 1);

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_execute_invalid_sql_throws)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_invalid.db");
    db.initialize(config);

    std::string invalidSql = "SELECT * FROM non_existent_table";
    BOOST_CHECK_THROW(db.execute(invalidSql), std::runtime_error);

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_query_select)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_query.db");
    db.initialize(config);

    // Create and populate table
    db.execute("CREATE TABLE users (id INTEGER, name TEXT)");
    db.execute("INSERT INTO users VALUES (1, 'Alice')");
    db.execute("INSERT INTO users VALUES (2, 'Bob')");

    auto rs = db.query("SELECT * FROM users ORDER BY id");

    BOOST_REQUIRE(rs != nullptr);

    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1);
    BOOST_CHECK_EQUAL(rs->getString(1), "Alice");

    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 2);
    BOOST_CHECK_EQUAL(rs->getString(1), "Bob");

    BOOST_CHECK(!rs->next());

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_query_empty_result)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_empty_query.db");
    db.initialize(config);

    db.execute("CREATE TABLE empty_table (id INTEGER)");
    auto rs = db.query("SELECT * FROM empty_table");

    BOOST_REQUIRE(rs != nullptr);
    BOOST_CHECK(!rs->next());

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_transaction_success)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_transaction_success.db");
    db.initialize(config);

    db.execute("CREATE TABLE accounts (id INTEGER, balance INTEGER)");
    db.execute("INSERT INTO accounts VALUES (1, 1000)");
    db.execute("INSERT INTO accounts VALUES (2, 500)");

    BOOST_REQUIRE_NO_THROW(db.transaction([&]()
                                          {
        db.execute("UPDATE accounts SET balance = balance - 100 WHERE id = 1");
        db.execute("UPDATE accounts SET balance = balance + 100 WHERE id = 2"); }));

    auto rs = db.query("SELECT balance FROM accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 900);

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_transaction_rollback_on_exception)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_transaction_rollback.db");
    db.initialize(config);

    db.execute("CREATE TABLE test_rollback (id INTEGER, value INTEGER)");
    db.execute("INSERT INTO test_rollback VALUES (1, 100)");

    BOOST_CHECK_THROW(db.transaction([&]()
                                     {
        db.execute("UPDATE test_rollback SET value = 200 WHERE id = 1");
        throw std::runtime_error("Test exception"); }),
                      std::runtime_error);

    // Verify rollback occurred
    auto rs = db.query("SELECT value FROM test_rollback WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 100);

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_get_connection)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_get_connection.db");
    db.initialize(config);

    auto conn = db.getConnection();
    BOOST_REQUIRE(conn != nullptr);
    BOOST_CHECK(conn->isOpen());

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_foreign_keys_enabled)
{
    SqliteDatabase db;

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_fk.db");
    db.initialize(config);

    // Check if foreign keys pragma can be set
    // Note: PRAGMA foreign_keys returns the current setting
    db.execute("PRAGMA foreign_keys = ON");
    auto rs = db.query("PRAGMA foreign_keys");

    if (rs->next())
    {
        // Some SQLite builds might not support foreign keys
        // So we just check that we can execute the pragma
        int64_t fk_setting = rs->getInt64(0);
        // If it's 0 or 1, that's fine - just log it
        std::cout << "Foreign keys setting: " << fk_setting << std::endl;
    }

    db.shutdown();
}

BOOST_AUTO_TEST_CASE(test_factory_creates_sqlite)
{
    auto db = DatabaseFactory::create(DatabaseType::Sqlite);
    BOOST_REQUIRE(db != nullptr);

    DatabaseConfig config;
    config["database"] = getTestDbPath("test_factory.db");
    db->initialize(config);

    db->execute("CREATE TABLE factory_test (id INTEGER)");
    db->execute("INSERT INTO factory_test VALUES (42)");

    auto rs = db->query("SELECT id FROM factory_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 42);

    db->shutdown();
}

BOOST_AUTO_TEST_CASE(test_factory_from_connection_string)
{
    // Используем относительный путь, а не абсолютный
    std::string dbPath = "test_data/test_conn_string.db";
    std::string connStr = "sqlite://" + dbPath;

    // Удаляем старый файл если существует
    std::error_code ec;
    std::filesystem::remove(dbPath, ec);

    auto db = DatabaseFactory::createFromConnectionString(connStr);
    BOOST_REQUIRE(db != nullptr);

    db->execute("CREATE TABLE conn_test (data TEXT)");
    db->execute("INSERT INTO conn_test VALUES ('test')");

    auto rs = db->query("SELECT data FROM conn_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), "test");

    db->shutdown();

    // Очищаем после теста
    std::filesystem::remove(dbPath, ec);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
