#pragma once

#include <filesystem>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "vault-server/src/storage/sqlite/sqlite_database.h"

namespace db::test
{

// Фикстура для Statement тестов - добавляем очистку таблицы перед каждым тестом
struct StatementTestFixture
{
    StatementTestFixture()
    {
        std::error_code ec;
        std::filesystem::remove("./test_data/statement_test.db", ec);

        config["database"] = "./test_data/statement_test.db";
        db.initialize(config);

        db.execute(
            "CREATE TABLE IF NOT EXISTS statement_test ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "age INTEGER, "
            "salary REAL, "
            "data BLOB, "
            "created DATETIME)"
        );
    }

    void clearTable()
    {
        db.execute("DELETE FROM statement_test");
        // Сброс автоинкремента
        auto rs = db.query(
            "SELECT name FROM sqlite_master WHERE type='table' AND name='statement_test'"
        );
        if (rs->next())
        {
            db.execute("DELETE FROM sqlite_sequence WHERE name='statement_test'");
        }
    }

    virtual ~StatementTestFixture()
    {
        try
        {
            db.shutdown();
            std::error_code ec;
            std::filesystem::remove("./test_data/statement_test.db", ec);
        }
        catch (...)
        {
            // игнорируем
        }
    }

    db::sqlite::SqliteDatabase db;
    DatabaseConfig config;
};

// Фикстура для ResultSet тестов
struct ResultSetTestFixture
{
    ResultSetTestFixture()
    {
        std::error_code ec;
        std::filesystem::remove("./test_data/resultset_test.db", ec);

        config["database"] = "./test_data/resultset_test.db";
        db.initialize(config);

        db.execute(
            "CREATE TABLE IF NOT EXISTS resultset_test ("
            "id INTEGER PRIMARY KEY, "
            "int_col INTEGER, "
            "float_col REAL, "
            "text_col TEXT, "
            "null_col INTEGER, "
            "blob_col BLOB)"
        );

        // Очищаем таблицу перед тестом
        db.execute("DELETE FROM resultset_test");

        // Вставка тестовых данных
        db.execute(
            "INSERT INTO resultset_test (id, int_col, float_col, text_col, null_col, blob_col) "
            "VALUES (1, 42, 3.14159, 'Hello World', NULL, X'010203FF')"
        );
        db.execute(
            "INSERT INTO resultset_test (id, int_col, float_col, text_col, null_col, blob_col) "
            "VALUES (2, 100, 2.71828, 'Second Row', NULL, NULL)"
        );
    }

    virtual ~ResultSetTestFixture()
    {
        try
        {
            db.shutdown();
            std::error_code ec;
            std::filesystem::remove("./test_data/resultset_test.db", ec);
        }
        catch (...)
        {
            // игнорируем
        }
    }

    db::sqlite::SqliteDatabase db;
    DatabaseConfig config;
};

// Фикстура для Transaction тестов
struct TransactionTestFixture
{
    TransactionTestFixture()
    {
        std::error_code ec;
        std::filesystem::remove("./test_data/transaction_test.db", ec);

        config["database"] = "./test_data/transaction_test.db";
        db.initialize(config);

        db.execute("CREATE TABLE IF NOT EXISTS tx_accounts ("
                   "id INTEGER PRIMARY KEY, "
                   "name TEXT, "
                   "balance INTEGER)");

        // Очищаем таблицу перед тестом
        db.execute("DELETE FROM tx_accounts");

        db.execute("INSERT INTO tx_accounts VALUES (1, 'Alice', 1000)");
        db.execute("INSERT INTO tx_accounts VALUES (2, 'Bob', 500)");
    }

    virtual ~TransactionTestFixture()
    {
        try
        {
            db.shutdown();
            std::error_code ec;
            std::filesystem::remove("./test_data/transaction_test.db", ec);
        }
        catch (...)
        {
            // игнорируем
        }
    }

    db::sqlite::SqliteDatabase db;
    DatabaseConfig config;
};

// Фикстура для Connection тестов
struct ConnectionTestFixture
{
    ConnectionTestFixture()
    {
        std::error_code ec;
        std::filesystem::remove("./test_data/connection_test.db", ec);

        config["database"] = "./test_data/connection_test.db";
        db.initialize(config);
    }

    virtual ~ConnectionTestFixture()
    {
        try
        {
            db.shutdown();
            std::error_code ec;
            std::filesystem::remove("./test_data/connection_test.db", ec);
        }
        catch (...)
        {
            // игнорируем
        }
    }

    db::sqlite::SqliteDatabase db;
    DatabaseConfig config;
};

} // namespace db::test
