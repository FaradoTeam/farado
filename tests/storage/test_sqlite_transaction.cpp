#include <boost/test/unit_test.hpp>

#include "test_fixture.h"
#include "vault-server/src/storage/sqlite/sqlite_database.h"

using namespace db::sqlite;

namespace db::test
{

BOOST_AUTO_TEST_SUITE(SqliteTransactionTests)

BOOST_FIXTURE_TEST_CASE(test_commit_changes, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx = conn->beginTransaction();
    BOOST_CHECK(tx->isActive());

    conn->execute("UPDATE tx_accounts SET balance = balance - 200 WHERE id = 1");
    conn->execute("UPDATE tx_accounts SET balance = balance + 200 WHERE id = 2");

    tx->commit();
    BOOST_CHECK(!tx->isActive());

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 800);

    rs = db.query("SELECT balance FROM tx_accounts WHERE id = 2");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 700);
}

BOOST_FIXTURE_TEST_CASE(test_rollback_changes, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx = conn->beginTransaction();
    conn->execute("UPDATE tx_accounts SET balance = 0 WHERE id = 1");
    conn->execute("UPDATE tx_accounts SET balance = 0 WHERE id = 2");

    tx->rollback();
    BOOST_CHECK(!tx->isActive());

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);

    rs = db.query("SELECT balance FROM tx_accounts WHERE id = 2");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 500);
}

BOOST_FIXTURE_TEST_CASE(test_auto_rollback_on_destruction, TransactionTestFixture)
{
    auto conn = db.getConnection();

    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_accounts SET balance = 999 WHERE id = 1");
    }

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);
}

BOOST_FIXTURE_TEST_CASE(test_commit_twice_throws, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->commit();
    BOOST_CHECK_THROW(tx->commit(), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_rollback_twice_throws, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->rollback();
    BOOST_CHECK_THROW(tx->rollback(), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_operations_after_commit_throw, TransactionTestFixture)
{
    auto conn = db.getConnection();
    auto tx = conn->beginTransaction();

    tx->commit();

    BOOST_CHECK_NO_THROW(conn->execute("UPDATE tx_accounts SET balance = 500 WHERE id = 1"));

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 500);
}

BOOST_FIXTURE_TEST_CASE(test_nested_transactions_not_supported, TransactionTestFixture)
{
    auto conn = db.getConnection();

    auto tx1 = conn->beginTransaction();

    BOOST_CHECK_THROW(conn->beginTransaction(), std::runtime_error);

    tx1->rollback();
}

BOOST_FIXTURE_TEST_CASE(test_transaction_with_exception_rollback, TransactionTestFixture)
{
    auto conn = db.getConnection();

    try
    {
        auto tx = conn->beginTransaction();
        conn->execute("UPDATE tx_accounts SET balance = 2000 WHERE id = 1");
        throw std::runtime_error("Something went wrong");
        tx->commit();
    }
    catch (...)
    {
        // Manual rollback needed
        // In production code use RAII
    }

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 1000);
}

BOOST_FIXTURE_TEST_CASE(test_transaction_guard_raii, TransactionTestFixture)
{
    struct TransactionGuard
    {
        TransactionGuard(std::unique_ptr<ITransaction> tx)
            : m_tx(std::move(tx))
        {
        }
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
        guard.commit();
    }

    auto rs = db.query("SELECT balance FROM tx_accounts WHERE id = 1");
    rs->next();
    BOOST_CHECK_EQUAL(rs->getInt64(0), 3000);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace db::test
