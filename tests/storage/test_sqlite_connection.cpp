#include <boost/test/unit_test.hpp>
#include "vault-server/src/storage/sqlite/sqlite_connection.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"
#include "test_fixture.h"

using namespace db::sqlite;

namespace db::test
{

// Тестовый набор для SqliteConnection
BOOST_AUTO_TEST_SUITE(SqliteConnectionTests)

/**
 * @brief Тест: открытие и закрытие соединения
 * 
 * Проверяет базовые операции с соединением: открытие, закрытие,
 * повторное открытие и проверку статуса isOpen().
 */
BOOST_FIXTURE_TEST_CASE(test_connection_open_close, ConnectionTestFixture)
{
    auto conn = db.getConnection();
    BOOST_CHECK(conn->isOpen());  // Соединение должно быть открыто после создания

    conn->close();
    BOOST_CHECK(!conn->isOpen());  // После закрытия - закрыто

    conn->open("test.db");  // Повторное открытие с новым файлом
    BOOST_CHECK(conn->isOpen());
}

/**
 * @brief Тест: выполнение SQL через соединение
 * 
 * Проверяет, что execute() корректно выполняет DDL и DML операции.
 */
BOOST_FIXTURE_TEST_CASE(test_execute_via_connection, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE conn_table (id INTEGER, name TEXT)");
    int64_t rows = conn->execute("INSERT INTO conn_table VALUES (1, 'Test')");

    BOOST_CHECK_EQUAL(rows, 1);  // Вставлена одна строка
}

/**
 * @brief Тест: создание подготовленного запроса
 * 
 * Проверяет создание Statement через prepareStatement() и
 * его использование для параметризованных запросов.
 */
BOOST_FIXTURE_TEST_CASE(test_prepare_statement, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE prep_test (id INTEGER, name TEXT)");

    // Создаём подготовленный запрос с параметрами
    auto stmt = conn->prepareStatement(
        "INSERT INTO prep_test (id, name) VALUES (@id, @name)"
    );

    BOOST_REQUIRE(stmt != nullptr);

    // Привязываем параметры и выполняем
    stmt->bindInt64("@id", 100);
    stmt->bindString("@name", "Prepared");
    stmt->execute();

    // Проверяем результат
    auto queryRs = db.query("SELECT name FROM prep_test WHERE id = 100");
    queryRs->next();
    BOOST_CHECK_EQUAL(queryRs->getString(0), "Prepared");
}

/**
 * @brief Тест: получение последнего вставленного ID
 * 
 * Проверяет работу getLastInsertId() после вставки в таблицу
 * с AUTOINCREMENT полем.
 */
BOOST_FIXTURE_TEST_CASE(test_get_last_insert_id, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE auto_id (id INTEGER PRIMARY KEY AUTOINCREMENT, data TEXT)");
    
    conn->execute("INSERT INTO auto_id (data) VALUES ('test1')");
    int64_t lastId = conn->getLastInsertId();
    BOOST_CHECK_EQUAL(lastId, 1);  // Первый ID = 1

    conn->execute("INSERT INTO auto_id (data) VALUES ('test2')");
    lastId = conn->getLastInsertId();
    BOOST_CHECK_EQUAL(lastId, 2);  // Второй ID = 2
}

/**
 * @brief Тест: экранирование спецсимволов в строках
 * 
 * Проверяет, что escapeString() корректно экранирует кавычки
 * для безопасного включения в SQL запросы.
 */
BOOST_FIXTURE_TEST_CASE(test_escape_string, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    // Строка с апострофом (кавычкой), который нужно экранировать
    std::string unsafe = "It's a 'quoted' string";
    std::string escaped = conn->escapeString(unsafe);

    // Используем экранированную строку в запросе
    conn->execute("CREATE TABLE escape_test (text TEXT)");
    std::string sql = "INSERT INTO escape_test VALUES ('" + escaped + "')";
    conn->execute(sql);

    // Проверяем, что данные сохранились корректно
    auto rs = db.query("SELECT text FROM escape_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getString(0), unsafe);
}

/**
 * @brief Тест: начало транзакции
 * 
 * Проверяет создание транзакции и фиксацию изменений.
 */
BOOST_FIXTURE_TEST_CASE(test_begin_transaction, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE tx_test (value INTEGER)");
    conn->execute("INSERT INTO tx_test VALUES (10)");

    // Начинаем транзакцию
    auto tx = conn->beginTransaction();
    BOOST_CHECK(tx->isActive());  // Транзакция активна

    conn->execute("UPDATE tx_test SET value = 20");

    tx->commit();  // Фиксируем изменения
    BOOST_CHECK(!tx->isActive());  // Транзакция больше не активна

    // Проверяем, что изменения применились
    auto rs = db.query("SELECT value FROM tx_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 20);
}

/**
 * @brief Тест: откат транзакции
 * 
 * Проверяет, что rollback() отменяет все изменения,
 * сделанные в рамках транзакции.
 */
BOOST_FIXTURE_TEST_CASE(test_transaction_rollback, ConnectionTestFixture)
{
    auto conn = db.getConnection();

    conn->execute("CREATE TABLE tx_rollback_test (value INTEGER)");
    conn->execute("INSERT INTO tx_rollback_test VALUES (100)");

    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_rollback_test SET value = 200");
        tx->rollback();  // Откатываем изменения
    }

    // Данные должны остаться неизменными
    auto rs = db.query("SELECT value FROM tx_rollback_test");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 100);
}

/**
 * @brief Тест: операции с закрытым соединением выбрасывают исключение
 * 
 * Проверяет, что все методы соединения корректно обрабатывают
 * ситуацию, когда соединение уже закрыто.
 */
BOOST_FIXTURE_TEST_CASE(test_connection_closed_operations_throw, ConnectionTestFixture)
{
    auto conn = db.getConnection();
    conn->close();

    // Все операции должны выбрасывать std::runtime_error
    BOOST_CHECK_THROW(conn->execute("SELECT 1"), std::runtime_error);
    BOOST_CHECK_THROW(conn->prepareStatement("SELECT 1"), std::runtime_error);
    BOOST_CHECK_THROW(conn->beginTransaction(), std::runtime_error);
    BOOST_CHECK_THROW(conn->getLastInsertId(), std::runtime_error);
    BOOST_CHECK_THROW(conn->escapeString("test"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
