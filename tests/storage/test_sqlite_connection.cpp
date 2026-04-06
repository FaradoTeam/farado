#include <boost/test/unit_test.hpp>

#include "vault-server/src/storage/sqlite/sqlite_connection.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

#include "test_fixture.h"

using namespace db::sqlite;

namespace db::test
{

BOOST_AUTO_TEST_SUITE(SqliteConnectionTests)

BOOST_FIXTURE_TEST_CASE(test_connection_open_close, ConnectionTestFixture)
{
    auto conn = db.getConnection();
    BOOST_CHECK(conn->isOpen());

    conn->close();
    BOOST_CHECK(!conn->isOpen());

    conn->open("test.db");
    BOOST_CHECK(conn->isOpen());
}

BOOST_FIXTURE_TEST_CASE(test_execute_via_connection, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE conn_table (id INTEGER, name TEXT)");
    int64_t rows = conn->execute("INSERT INTO conn_table VALUES (1, 'Test')");

    BOOST_CHECK_EQUAL(rows, 1);
}

BOOST_FIXTURE_TEST_CASE(test_prepare_statement, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE prep_test (id INTEGER, name TEXT)");

    auto stmt = conn->prepareStatement(
        "INSERT INTO prep_test (id, name) VALUES (@id, @name)"
    );

    BOOST_REQUIRE(stmt != nullptr);

    stmt->bindInt64("@id", 100);
    stmt->bindString("@name", "Prepared");
    stmt->execute();

    auto queryRs = db.query("SELECT name FROM prep_test WHERE id = 100");
    queryRs->next();
    BOOST_CHECK_EQUAL(queryRs->getString(0), "Prepared");
}

BOOST_FIXTURE_TEST_CASE(test_get_last_insert_id, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE auto_id (id INTEGER PRIMARY KEY AUTOINCREMENT, data TEXT)");
    conn->execute("INSERT INTO auto_id (data) VALUES ('test1')");

    int64_t lastId = conn->getLastInsertId();
    BOOST_CHECK_EQUAL(lastId, 1);

    conn->execute("INSERT INTO auto_id (data) VALUES ('test2')");
    lastId = conn->getLastInsertId();
    BOOST_CHECK_EQUAL(lastId, 2);
}

BOOST_FIXTURE_TEST_CASE(test_escape_string, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    std::string unsafe = "It's a 'quoted' string";
    std::string escaped = conn->escapeString(unsafe);

    conn->execute("CREATE TABLE escape_test (text TEXT)");
    std::string sql = "INSERT INTO escape_test VALUES ('" + escaped + "')";
    conn->execute(sql);

    auto rs = db.query("SELECT text FROM escape_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), unsafe);
}

BOOST_FIXTURE_TEST_CASE(test_begin_transaction, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE tx_test (value INTEGER)");
    conn->execute("INSERT INTO tx_test VALUES (10)");

    auto tx = conn->beginTransaction();
    BOOST_CHECK(tx->isActive());

    conn->execute("UPDATE tx_test SET value = 20");

    tx->commit();
    BOOST_CHECK(!tx->isActive());

    auto rs = db.query("SELECT value FROM tx_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 20);
}

BOOST_FIXTURE_TEST_CASE(test_transaction_rollback, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE tx_rollback_test (value INTEGER)");
    conn->execute("INSERT INTO tx_rollback_test VALUES (100)");

    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_rollback_test SET value = 200");
        tx->rollback();
    }

    auto rs = db.query("SELECT value FROM tx_rollback_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 100);
}

BOOST_FIXTURE_TEST_CASE(test_connection_closed_operations_throw, ConnectionTestFixture)
{
    auto conn = db.getConnection();
    conn->close();

    BOOST_CHECK_THROW(conn->execute("SELECT 1"), std::runtime_error);
    BOOST_CHECK_THROW(conn->prepareStatement("SELECT 1"), std::runtime_error);
    BOOST_CHECK_THROW(conn->beginTransaction(), std::runtime_error);
    BOOST_CHECK_THROW(conn->getLastInsertId(), std::runtime_error);
    BOOST_CHECK_THROW(conn->escapeString("test"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
