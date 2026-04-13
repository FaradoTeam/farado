#pragma once

#include <atomic>
#include <memory>
#include <string>

#include "idatabase.h"

namespace db
{

/**
 * @brief Реализация IDatabase для SQLite.
 *
 * @details Управляет конфигурацией SQLite БД и предоставляет соединения.
 *          Особенность SQLite в том, что он не поддерживает несколько
 *          одновременных соединений с одной БД из одного процесса
 *          (хотя технически возможно, но требует осторожности).
 *
 * @note В текущей реализации разрешено только одно активное соединение.
 *       Повторный вызов connection() до shutdown() предыдущего соединения
 *       приведёт к исключению.
 */
class SqliteDatabase final : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase() override;

    // Запрещаем копирование и перемещение, чтобы избежать дублирования соединений
    SqliteDatabase(const SqliteDatabase&) = delete;
    SqliteDatabase& operator=(const SqliteDatabase&) = delete;
    SqliteDatabase(SqliteDatabase&&) = delete;
    SqliteDatabase& operator=(SqliteDatabase&&) = delete;

    // Реализация IDatabase
    void initialize(const DatabaseConfig& config) override;
    void shutdown() override;
    std::shared_ptr<IConnection> connection() override;

private:
    /// Путь к файлу БД
    std::string m_dbPath;

    /// Флаг инициализации
    std::atomic<bool> m_initialized;

    /// Слабая ссылка на активное соединение
    std::weak_ptr<IConnection> m_activeConnection;
};

} // namespace db
