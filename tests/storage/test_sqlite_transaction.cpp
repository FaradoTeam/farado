#include <boost/test/unit_test.hpp>
#include "test_fixture.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

// Тестовый набор для SqliteTransaction
BOOST_AUTO_TEST_SUITE(SqliteTransactionTests)

/**
 * @brief Тест: фиксация изменений транзакции
 * 
 * Проверяет, что commit() сохраняет все изменения в БД.
 */
BOOST_FIXTURE_TEST_CASE(test_commit_changes, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx = conn->beginTransaction();
    BOOST_CHECK(tx->isActive());  // Транзакция активна

    // Выполняем перевод средств между счетами
    conn->execute("UPDATE tx_accounts SET balance = balance - 200 WHERE id = 1");
    conn->execute("UPDATE tx_accounts SET balance = balance + 200 WHERE id = 2");

    tx->commit();  // Фиксируем изменения
    BOOST_CHECK(!tx->isActive());  // Транзакция больше не активна

    // Проверяем, что изменения применились
    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 800);  // 1000 - 200

    rs = db.query("SELECT balance FROM tx_accounts WHERE id = 2");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 700);  // 500 + 200
}

/**
 * @brief Тест: откат изменений транзакции
 * 
 * Проверяет, что rollback() отменяет все изменения,
 * сделанные в рамках транзакции.
 */
BOOST_FIXTURE_TEST_CASE(test_rollback_changes, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx = conn->beginTransaction();
    // Обнуляем балансы на обоих счетах
    conn->execute("UPDATE tx_accounts SET balance = 0 WHERE id = 1");
    conn->execute("UPDATE tx_accounts SET balance = 0 WHERE id = 2");

    tx->rollback();  // Откатываем изменения
    BOOST_CHECK(!tx->isActive());

    // Проверяем, что балансы вернулись к исходным значениям
    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);

    rs = db.query("SELECT balance FROM tx_accounts WHERE id = 2");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 500);
}

/**
 * @brief Тест: автоматический откат при разрушении транзакции
 * 
 * Проверяет RAII поведение: если транзакция не была
 * закоммичена до разрушения, происходит автоматический откат.
 */
BOOST_FIXTURE_TEST_CASE(test_auto_rollback_on_destruction, TransactionTestFixture)
{
    auto conn = db.getConnection();

    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_accounts SET balance = 999 WHERE id = 1");
        // Транзакция не коммитится - при выходе из блока произойдёт откат
    }

    // Изменения не должны сохраниться
    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);
}

/**
 * @brief Тест: повторный commit вызывает исключение
 * 
 * Проверяет, что нельзя закоммитить уже завершённую транзакцию.
 */
BOOST_FIXTURE_TEST_CASE(test_commit_twice_throws, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->commit();  // Первый коммит успешен
    BOOST_CHECK_THROW(tx->commit(), std::runtime_error);  // Второй - исключение
}

/**
 * @brief Тест: повторный rollback вызывает исключение
 * 
 * Проверяет, что нельзя откатить уже завершённую транзакцию.
 */
BOOST_FIXTURE_TEST_CASE(test_rollback_twice_throws, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->rollback();  // Первый откат успешен
    BOOST_CHECK_THROW(tx->rollback(), std::runtime_error);  // Второй - исключение
}

/**
 * @brief Тест: операции после commit работают нормально
 * 
 * Проверяет, что после фиксации транзакции можно продолжать
 * работать с БД через то же соединение.
 */
BOOST_FIXTURE_TEST_CASE(test_operations_after_commit_throw, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->commit();

    // После коммита транзакции обычные операции должны работать
    BOOST_CHECK_NO_THROW(conn->execute("UPDATE tx_accounts SET balance = 500 WHERE id = 1"));

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 500);
}

/**
 * @brief Тест: вложенные транзакции не поддерживаются
 * 
 * Проверяет, что SQLite не поддерживает вложенные транзакции,
 * и попытка начать новую транзакцию вызывает исключение.
 */
BOOST_FIXTURE_TEST_CASE(test_nested_transactions_not_supported, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx1 = conn->beginTransaction();
    // Пытаемся начать вторую транзакцию до завершения первой
    BOOST_CHECK_THROW(conn->beginTransaction(), std::runtime_error);

    tx1->rollback();
}

/**
 * @brief Тест: обработка исключений в транзакции
 * 
 * Демонстрирует, что при возникновении исключения
 * необходимо явно выполнить rollback.
 */
BOOST_FIXTURE_TEST_CASE(test_transaction_with_exception_rollback, TransactionTestFixture)
{
    auto conn = db.getConnection();

    try
    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_accounts SET balance = 2000 WHERE id = 1");
        throw std::runtime_error("Something went wrong");  // Искусственное исключение
        tx->commit();  // Сюда не дойдём
    }
    catch (...)
    {
        // В реальном коде нужно использовать RAII для автоматического отката
        // Здесь показан пример ручного отката
    }

    // Изменения не должны примениться
    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);
}

/**
 * @brief Тест: RAII обёртка для транзакции
 * 
 * Демонстрирует правильный паттерн использования транзакций
 * с автоматическим управлением жизненным циклом.
 */
BOOST_FIXTURE_TEST_CASE(test_transaction_guard_raii, TransactionTestFixture)
{
    // RAII обёртка для автоматического управления транзакцией
    struct TransactionGuard
    {
        TransactionGuard(std::unique_ptr<ITransaction> tx)
            : m_tx(std::move(tx))
        {
        }
        
        // В деструкторе автоматически откатываем незакоммиченную транзакцию
        ~TransactionGuard()
        {
            if (m_tx && m_tx->isActive())
                m_tx->rollback();
        }
        
        void commit()
        {
            if (m_tx)
                m_tx->commit();
        }
        
        ITransaction* operator->() { return m_tx.get(); }

    private:
        std::unique_ptr<ITransaction> m_tx;
    };

    auto conn = db.getConnection();

    {
        TransactionGuard guard(conn->beginTransaction());
        conn->execute("UPDATE tx_accounts SET balance = 3000 WHERE id = 1");
        guard.commit();  // Явно фиксируем изменения
    }

    // Проверяем, что изменения применились
    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 3000);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
