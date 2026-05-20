#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "common/log/log.h"

#include "storage/idatabase.h"

#include "sqlite_workflow_repository.h"

namespace
{

/**
 * @brief Преобразует строку результата в объект Workflow.
 */
dto::Workflow mapRowToWorkflow(db::IResultSet& rs)
{
    dto::Workflow workflow;
    workflow.id = rs.valueInt64("id");
    workflow.caption = rs.valueString("caption");

    if (!rs.isNull("description"))
        workflow.description = rs.valueString("description");

    return workflow;
}

} // namespace

namespace server
{
namespace repositories
{

SqliteWorkflowRepository::SqliteWorkflowRepository(std::shared_ptr<db::IDatabase> database)
    : m_database(std::move(database))
{
    if (!m_database)
    {
        throw std::runtime_error("SqliteWorkflowRepository: database is null");
    }
}

std::pair<std::vector<dto::Workflow>, int64_t>
SqliteWorkflowRepository::findAll(int page, int pageSize)
{
    std::vector<dto::Workflow> workflows;
    int64_t totalCount = 0;

    try
    {
        auto conn = m_database->connection();

        // Получаем общее количество
        auto countStmt = conn->prepareStatement("SELECT COUNT(*) FROM Workflow");
        auto countRs = countStmt->executeQuery();
        if (countRs->next())
        {
            totalCount = countRs->valueInt64(0);
        }

        if (totalCount == 0)
        {
            return { workflows, totalCount };
        }

        // Получаем страницу с рабочими процессами
        const int offset = (page - 1) * pageSize;
        auto stmt = conn->prepareStatement(
            "SELECT id, caption, description "
            "FROM Workflow ORDER BY id LIMIT :limit OFFSET :offset"
        );

        stmt->bindInt64("limit", pageSize);
        stmt->bindInt64("offset", offset);
        auto rs = stmt->executeQuery();

        while (rs->next())
        {
            workflows.push_back(mapRowToWorkflow(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения списка рабочих процессов: " << e.what();
        throw;
    }

    return { workflows, totalCount };
}

std::optional<dto::Workflow> SqliteWorkflowRepository::findById(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "findById: неверный идентификатор " << id;
        return std::nullopt;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, caption, description FROM Workflow WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        if (rs->next())
        {
            return mapRowToWorkflow(*rs);
        }

        return std::nullopt;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка поиска рабочего процесса по id: " << e.what();
        throw;
    }
}

int64_t SqliteWorkflowRepository::create(const dto::Workflow& workflow)
{
    if (!workflow.caption.has_value() || workflow.caption->empty())
    {
        LOG_WARN
            << "Создание рабочего процесса: отсутствует обязательное поле caption";
        return 0;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "INSERT INTO Workflow (caption, description) "
            "VALUES (:caption, :description)"
        );

        stmt->bindString("caption", workflow.caption.value());

        if (workflow.description.has_value())
            stmt->bindString("description", *workflow.description);
        else
            stmt->bindNull("description");

        stmt->execute();
        return conn->lastInsertId();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка создания рабочего процесса: " << e.what();
        throw;
    }
}

bool SqliteWorkflowRepository::update(const dto::Workflow& workflow)
{
    if (!workflow.id.has_value())
    {
        LOG_WARN << "update: отсутствует ID рабочего процесса";
        return false;
    }

    try
    {
        auto conn = m_database->connection();

        std::string sql = "UPDATE Workflow SET ";
        std::vector<std::string> setClauses;

        if (workflow.caption.has_value())
            setClauses.push_back("caption = :caption");
        if (workflow.description.has_value())
            setClauses.push_back("description = :description");

        if (setClauses.empty())
        {
            LOG_WARN << "update: нет полей для обновления";
            return false;
        }

        sql += boost::algorithm::join(setClauses, ", ");
        sql += " WHERE id = :id";

        auto stmt = conn->prepareStatement(sql);

        if (workflow.caption.has_value())
            stmt->bindString("caption", *workflow.caption);
        if (workflow.description.has_value())
            stmt->bindString("description", *workflow.description);

        stmt->bindInt64("id", *workflow.id);

        int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка обновления рабочего процесса: " << e.what();
        return false;
    }
}

bool SqliteWorkflowRepository::remove(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "remove: неверный идентификатор " << id;
        return false;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement("DELETE FROM Workflow WHERE id = :id");
        stmt->bindInt64("id", id);

        int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка удаления рабочего процесса: " << e.what();
        return false;
    }
}

bool SqliteWorkflowRepository::existsByCaption(const std::string& caption)
{
    if (caption.empty())
        return false;

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT 1 FROM Workflow WHERE caption = :caption LIMIT 1"
        );

        stmt->bindString("caption", caption);
        auto rs = stmt->executeQuery();

        return rs->next();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR
            << "Ошибка проверки существования рабочего процесса по названию: "
            << e.what();
        return false;
    }
}

std::shared_ptr<db::IConnection> SqliteWorkflowRepository::connection() const
{
    return m_database->connection();
}

} // namespace repositories
} // namespace server
