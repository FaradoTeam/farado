#include <boost/test/unit_test.hpp>
#include "test_fixture.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

// Тестовый набор для SqliteResultSet
BOOST_AUTO_TEST_SUITE(SqliteResultSetTests)

/**
 * @brief Тест: навигация по строкам результата
 * 
 * Проверяет работу next() для последовательного перебора строк.
 */
BOOST_FIXTURE_TEST_CASE(test_next_and_row_iteration, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test ORDER BY id");

    // Первая строка (id=1)
    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1);

    // Вторая строка (id=2)
    BOOST_CHECK(rs->next());
    BOOST_CHECK_EQUAL(rs->getInt64(0), 2);

    // Больше строк нет
    BOOST_CHECK(!rs->next());
}

/**
 * @brief Тест: получение количества колонок
 * 
 * Проверяет, что getColumnCount() возвращает правильное число.
 */
BOOST_FIXTURE_TEST_CASE(test_get_column_count, ResultSetTestFixture)
{
    auto rs = db.query("SELECT * FROM resultset_test");
    BOOST_CHECK_EQUAL(rs->getColumnCount(), 6);  // В таблице 6 колонок
}

/**
 * @brief Тест: получение имени и индекса колонки
 * 
 * Проверяет преобразование между именем колонки и её индексом.
 */
BOOST_FIXTURE_TEST_CASE(test_get_column_name_and_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id, int_col, text_col FROM resultset_test");

    // Проверяем имена колонок по индексу
    BOOST_CHECK_EQUAL(rs->getColumnName(0), "id");
    BOOST_CHECK_EQUAL(rs->getColumnName(1), "int_col");
    BOOST_CHECK_EQUAL(rs->getColumnName(2), "text_col");

    // Проверяем индексы по имени
    BOOST_CHECK_EQUAL(rs->getColumnIndex("id"), 0);
    BOOST_CHECK_EQUAL(rs->getColumnIndex("int_col"), 1);
    BOOST_CHECK_EQUAL(rs->getColumnIndex("text_col"), 2);
}

/**
 * @brief Тест: получение значения по индексу
 * 
 * Проверяет универсальный метод getValue() с указанием индекса.
 */
BOOST_FIXTURE_TEST_CASE(test_get_value_by_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT int_col, float_col, text_col FROM resultset_test WHERE id = 1");
    rs->next();

    // Целочисленное значение
    FieldValue intVal = rs->getValue(0);
    BOOST_CHECK_EQUAL(intVal.asInt64(), 42);

    // Вещественное значение
    FieldValue floatVal = rs->getValue(1);
    BOOST_CHECK_CLOSE(floatVal.asDouble(), 3.14159, 0.00001);

    // Текстовое значение
    FieldValue textVal = rs->getValue(2);
    BOOST_CHECK_EQUAL(textVal.asString(), "Hello World");
}

/**
 * @brief Тест: получение значения по имени колонки
 * 
 * Проверяет универсальный метод getValue() с указанием имени.
 */
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

/**
 * @brief Тест: типобезопасные геттеры
 * 
 * Проверяет методы getInt64(), getDouble(), getString()
 * как по индексу, так и по имени колонки.
 */
BOOST_FIXTURE_TEST_CASE(test_type_safe_getters, ResultSetTestFixture)
{
    auto rs = db.query("SELECT int_col, float_col, text_col FROM resultset_test WHERE id = 1");
    rs->next();

    // Целые числа
    BOOST_CHECK_EQUAL(rs->getInt64(0), 42);
    BOOST_CHECK_EQUAL(rs->getInt64("int_col"), 42);

    // Числа с плавающей точкой
    BOOST_CHECK_CLOSE(rs->getDouble(1), 3.14159, 0.00001);
    BOOST_CHECK_CLOSE(rs->getDouble("float_col"), 3.14159, 0.00001);

    // Строки
    BOOST_CHECK_EQUAL(rs->getString(2), "Hello World");
    BOOST_CHECK_EQUAL(rs->getString("text_col"), "Hello World");
}

/**
 * @brief Тест: проверка на NULL
 * 
 * Проверяет, что isNull() корректно определяет NULL значения.
 */
BOOST_FIXTURE_TEST_CASE(test_is_null, ResultSetTestFixture)
{
    auto rs = db.query("SELECT null_col FROM resultset_test WHERE id = 1");
    rs->next();

    BOOST_CHECK(rs->isNull(0));
    BOOST_CHECK(rs->isNull("null_col"));
}

/**
 * @brief Тест: получение BLOB данных
 * 
 * Проверяет чтение бинарных данных из БД.
 */
BOOST_FIXTURE_TEST_CASE(test_get_blob, ResultSetTestFixture)
{
    auto rs = db.query("SELECT blob_col FROM resultset_test WHERE id = 1");
    rs->next();

    // Ожидаемые данные: 0x01, 0x02, 0x03, 0xFF
    Blob expected = { 0x01, 0x02, 0x03, 0xFF };
    
    // Чтение по индексу
    Blob actual = rs->getBlob(0);
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());

    // Чтение по имени
    Blob actualByName = rs->getBlob("blob_col");
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actualByName.begin(), actualByName.end());
}

/**
 * @brief Тест: NULL BLOB
 * 
 * Проверяет, что попытка получить NULL как BLOB вызывает исключение.
 */
BOOST_FIXTURE_TEST_CASE(test_null_blob, ResultSetTestFixture)
{
    auto rs = db.query("SELECT blob_col FROM resultset_test WHERE id = 2");
    rs->next();

    BOOST_CHECK(rs->isNull(0));  // Значение NULL
    BOOST_CHECK_THROW(rs->getBlob(0), std::runtime_error);  // Исключение при попытке чтения
}

/**
 * @brief Тест: невалидный индекс колонки
 * 
 * Проверяет обработку ошибок при доступе по несуществующему индексу.
 */
BOOST_FIXTURE_TEST_CASE(test_invalid_column_index, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");
    rs->next();

    BOOST_CHECK_THROW(rs->getColumnName(100), std::runtime_error);
    BOOST_CHECK_THROW(rs->getValue(100), std::runtime_error);
}

/**
 * @brief Тест: невалидное имя колонки
 * 
 * Проверяет обработку ошибок при доступе по несуществующему имени.
 */
BOOST_FIXTURE_TEST_CASE(test_invalid_column_name, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");
    rs->next();

    BOOST_CHECK_THROW(rs->getColumnIndex("non_existent"), std::runtime_error);
    BOOST_CHECK_THROW(rs->getValue("non_existent"), std::runtime_error);
}

/**
 * @brief Тест: кэширование индексов колонок
 * 
 * Проверяет, что повторное получение индекса по имени
 * использует кэш для оптимизации.
 */
BOOST_FIXTURE_TEST_CASE(test_column_index_caching, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id, int_col, text_col FROM resultset_test");

    int idx1 = rs->getColumnIndex("text_col");
    int idx2 = rs->getColumnIndex("text_col");

    BOOST_CHECK_EQUAL(idx1, idx2);  // Оба раза вернулся одинаковый индекс
    BOOST_CHECK_EQUAL(idx1, 2);     // Индекс колонки text_col
}

/**
 * @brief Тест: вызов getValue() до next() вызывает исключение
 * 
 * Проверяет, что нельзя читать данные до первого вызова next().
 */
BOOST_FIXTURE_TEST_CASE(test_get_value_before_next_throws, ResultSetTestFixture)
{
    auto rs = db.query("SELECT id FROM resultset_test");
    // rs->next() не вызывался
    BOOST_CHECK_THROW(rs->getValue(0), std::runtime_error);
}

/**
 * @brief Тест: несоответствие типов вызывает исключение
 * 
 * Проверяет, что попытка прочитать число как строку (и наоборот)
 * приводит к исключению.
 */
BOOST_FIXTURE_TEST_CASE(test_type_mismatch_throws, ResultSetTestFixture)
{
    auto rs = db.query("SELECT text_col FROM resultset_test WHERE id = 1");
    rs->next();

    // Пытаемся прочитать текст как число
    BOOST_CHECK_THROW(rs->getInt64(0), std::runtime_error);
    BOOST_CHECK_THROW(rs->getDouble(0), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
