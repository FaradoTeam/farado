#include <boost/test/unit_test.hpp>

#include "test_fixture.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

BOOST_AUTO_TEST_SUITE(SqliteResultSetTests)

BOOST_FIXTURE_TEST_CASE(test_next_and_row_iteration, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test ORDER BY id");

    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1);

    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 2);

    BOOST_CHECK(!rs->next());
}

BOOST_FIXTURE_TEST_CASE(test_get_column_count, ResultSetTestFixture)
{
    auto rs = db.query("SELECT * FROM resultset_test");
    BOOST_CHECK_EQUAL(rs->getColumnCount(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_get_column_name_and_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id, int_col, text_col FROM resultset_test");

    BOOST_CHECK_EQUAL(rs->getColumnName(0), "id");
    BOOST_CHECK_EQUAL(rs->getColumnName(1), "int_col");
    BOOST_CHECK_EQUAL(rs->getColumnName(2), "text_col");

    BOOST_CHECK_EQUAL(rs->getColumnIndex("id"), 0);
    BOOST_CHECK_EQUAL(rs->getColumnIndex("int_col"), 1);
    BOOST_CHECK_EQUAL(rs->getColumnIndex("text_col"), 2);
}

BOOST_FIXTURE_TEST_CASE(test_get_value_by_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT int_col, float_col, text_col FROM resultset_test WHERE id = 1");
    rs->next();

    FieldValue intVal = rs->getValue(0);
    BOOST_CHECK_EQUAL(intVal.asInt64(), 42);

    FieldValue floatVal = rs->getValue(1);
    BOOST_CHECK_CLOSE(floatVal.asDouble(), 3.14159, 0.00001);

    FieldValue textVal = rs->getValue(2);
    BOOST_CHECK_EQUAL(textVal.asString(), "Hello World");
}

BOOST_FIXTURE_TEST_CASE(test_get_value_by_name, ResultSetTestFixture)
{
    auto rs = db.query("SELECT int_col, float_col, text_col FROM resultset_test WHERE id = 1");
    rs->next();

    FieldValue intVal = rs->getValue("int_col");
    BOOST_CHECK_EQUAL(intVal.asInt64(), 42);

    FieldValue floatVal = rs->getValue("float_col");
    BOOST_CHECK_CLOSE(floatVal.asDouble(), 3.14159, 0.00001);

    FieldValue textVal = rs->getValue("text_col");
    BOOST_CHECK_EQUAL(textVal.asString(), "Hello World");
}

BOOST_FIXTURE_TEST_CASE(test_type_safe_getters, ResultSetTestFixture)
{
    auto rs = db.query("SELECT int_col, float_col, text_col FROM resultset_test WHERE id = 1");
    rs->next();

    BOOST_CHECK_EQUAL(rs->getInt64(0), 42);
    BOOST_CHECK_EQUAL(rs->getInt64("int_col"), 42);

    BOOST_CHECK_CLOSE(rs->getDouble(1), 3.14159, 0.00001);
    BOOST_CHECK_CLOSE(rs->getDouble("float_col"), 3.14159, 0.00001);

    BOOST_CHECK_EQUAL(rs->getString(2), "Hello World");
    BOOST_CHECK_EQUAL(rs->getString("text_col"), "Hello World");
}

BOOST_FIXTURE_TEST_CASE(test_is_null, ResultSetTestFixture)
{
    auto rs = db.query("SELECT null_col FROM resultset_test WHERE id = 1");
    rs->next();

    BOOST_CHECK(rs->isNull(0));
    BOOST_CHECK(rs->isNull("null_col"));
}

BOOST_FIXTURE_TEST_CASE(test_get_blob, ResultSetTestFixture)
{
    auto rs = db.query("SELECT blob_col FROM resultset_test WHERE id = 1");
    rs->next();

    Blob expected = { 0x01, 0x02, 0x03, 0xFF };
    Blob actual = rs->getBlob(0);

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());

    Blob actualByName = rs->getBlob("blob_col");
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actualByName.begin(), actualByName.end());
}

BOOST_FIXTURE_TEST_CASE(test_null_blob, ResultSetTestFixture)
{
    auto rs = db.query("SELECT blob_col FROM resultset_test WHERE id = 2");
    rs->next();

    BOOST_CHECK(rs->isNull(0));
    BOOST_CHECK_THROW(rs->getBlob(0), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_invalid_column_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");
    rs->next();

    BOOST_CHECK_THROW(rs->getColumnName(100), std::runtime_error);
    BOOST_CHECK_THROW(rs->getValue(100), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_invalid_column_name, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");
    rs->next();

    BOOST_CHECK_THROW(rs->getColumnIndex("non_existent"), std::runtime_error);
    BOOST_CHECK_THROW(rs->getValue("non_existent"), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_column_index_caching, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id, int_col, text_col FROM resultset_test");

    int idx1 = rs->getColumnIndex("text_col");
    int idx2 = rs->getColumnIndex("text_col");

    BOOST_CHECK_EQUAL(idx1, idx2);
    BOOST_CHECK_EQUAL(idx1, 2);
}

BOOST_FIXTURE_TEST_CASE(test_get_value_before_next_throws, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");

    BOOST_CHECK_THROW(rs->getValue(0), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_type_mismatch_throws, ResultSetTestFixture)
{
    auto rs = db.query("SELECT text_col FROM resultset_test WHERE id = 1");
    rs->next();

    BOOST_CHECK_THROW(rs->getInt64(0), std::runtime_error);
    BOOST_CHECK_THROW(rs->getDouble(0), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
