#include <memory>
#include <stdexcept>

#include "sqlite_connection.h"
#include "sqlite_database.h"

namespace db
{

SqliteDatabase::SqliteDatabase()
    : m_initialized(false)
{
    // Пустая инициализация - реальная настройка произойдёт в initialize()
}

SqliteDatabase::~SqliteDatabase()
{
    // Если БД была инициализирована, корректно завершаем работу
    if (m_initialized)
    {
        shutdown();
    }
}

void SqliteDatabase::initialize(const DatabaseConfig& config)
{
    if (m_initialized)
    {
        throw std::runtime_error("Database already initialized");
    }

    // Проверяем наличие обязательного параметра "database" (путь к файлу)
    auto it = config.find("database");
    if (it == config.end())
    {
        throw std::runtime_error("Database path not specified in config");
    }

    // Сохраняем путь к файлу БД
    m_dbPath = it->second;
    m_initialized = true;
}

void SqliteDatabase::shutdown()
{
    // Сбрасываем флаг инициализации
    m_initialized = false;

    // Освобождаем активное соединение, если оно ещё существует
    m_activeConnection.reset();
}

std::shared_ptr<IConnection> SqliteDatabase::connection()
{
    if (!m_initialized)
    {
        throw std::runtime_error("Database not initialized");
    }

    // Проверяем, существует ли уже активное соединение
    if (m_activeConnection.lock())
    {
        // SQLite не очень хорошо подходит для доступа с несколькими
        // соединениями, поэтому ограничиваемся одним
        throw std::runtime_error(
            "SQLite does not support multiple connections. "
            "Only one connection can be active at a time."
        );
    }

    // Создаём новое соединение
    auto result = std::make_shared<SqliteConnection>(m_dbPath);
    m_activeConnection = result;
    return result;
}

} // namespace db
