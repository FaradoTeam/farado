#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "common/log/log.h"

#include "storage/idatabase.h"

#include "sqlite_state_repository.h"

namespace server
{
namespace repositories
{

dto::State SqliteStateRepository::mapRowToState(db::IResultSet& rs)
{
    dto::State state;
    state.id = rs.valueInt64("id");
    state.workflowId = rs.valueInt64("workflowId");
    state.caption = rs.valueString("caption");

    if (!rs.isNull("description"))
        state.description = rs.valueString("description");

    state.weight = rs.valueInt64("weight");
    state.orderNumber = rs.valueInt64("orderNumber");
    state.isArchive = rs.valueInt64("isArchive") != 0;
    state.isQueue = rs.valueInt64("isQueue") != 0;

    return state;
}

SqliteStateRepository::SqliteStateRepository(std::shared_ptr<db::IDatabase> database)
    : m_database(std::move(database))
{
    if (!m_database)
    {
        throw std::runtime_error("SqliteStateRepository: database is null");
    }
}

std::pair<std::vector<dto::State>, int64_t>
SqliteStateRepository::findAll(int page, int pageSize, std::optional<int64_t> workflowId)
{
    std::vector<dto::State> states;
    int64_t totalCount = 0;

    try
    {
        auto conn = m_database->connection();

        // Формируем SQL с учётом фильтра
        std::string countSql = "SELECT COUNT(*) FROM State";
        std::string selectSql = "SELECT id, workflowId, caption, description, weight, "
                                "orderNumber, isArchive, isQueue FROM State";

        if (workflowId.has_value())
        {
            countSql += " WHERE workflowId = :workflowId";
            selectSql += " WHERE workflowId = :workflowId";
        }

        selectSql += " ORDER BY workflowId, orderNumber LIMIT :limit OFFSET :offset";

        // Получаем общее количество
        auto countStmt = conn->prepareStatement(countSql);
        if (workflowId.has_value())
        {
            countStmt->bindInt64("workflowId", *workflowId);
        }
        auto countRs = countStmt->executeQuery();
        if (countRs->next())
        {
            totalCount = countRs->valueInt64(0);
        }

        if (totalCount == 0)
        {
            return { states, totalCount };
        }

        // Получаем страницу с состояниями
        const int offset = (page - 1) * pageSize;
        auto stmt = conn->prepareStatement(selectSql);

        if (workflowId.has_value())
        {
            stmt->bindInt64("workflowId", *workflowId);
        }
        stmt->bindInt64("limit", pageSize);
        stmt->bindInt64("offset", offset);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            states.push_back(mapRowToState(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения списка состояний: " << e.what();
        throw;
    }

    return { states, totalCount };
}

std::optional<dto::State> SqliteStateRepository::findById(int64_t id)
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
            "SELECT id, workflowId, caption, description, weight, "
            "orderNumber, isArchive, isQueue FROM State WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        if (rs->next())
        {
            return mapRowToState(*rs);
        }

        return std::nullopt;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка поиска состояния по id: " << e.what();
        throw;
    }
}

int64_t SqliteStateRepository::create(const dto::State& state)
{
    if (!state.workflowId.has_value() || !state.caption.has_value() || state.caption->empty())
    {
        LOG_WARN << "Создание состояния: отсутствуют обязательные поля";
        return 0;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "INSERT INTO State (workflowId, caption, description, weight, "
            "orderNumber, isArchive, isQueue) "
            "VALUES (:workflowId, :caption, :description, :weight, "
            ":orderNumber, :isArchive, :isQueue)"
        );

        stmt->bindInt64("workflowId", *state.workflowId);
        stmt->bindString("caption", *state.caption);

        if (state.description.has_value())
            stmt->bindString("description", *state.description);
        else
            stmt->bindNull("description");

        stmt->bindInt64("weight", state.weight.value_or(0));
        stmt->bindInt64("orderNumber", state.orderNumber.value_or(0));
        stmt->bindInt64("isArchive", state.isArchive.value_or(false) ? 1 : 0);
        stmt->bindInt64("isQueue", state.isQueue.value_or(false) ? 1 : 0);

        stmt->execute();
        return conn->lastInsertId();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка создания состояния: " << e.what();
        throw;
    }
}

bool SqliteStateRepository::update(const dto::State& state)
{
    if (!state.id.has_value())
    {
        LOG_WARN << "update: отсутствует ID состояния";
        return false;
    }

    try
    {
        auto conn = m_database->connection();

        std::string sql = "UPDATE State SET ";
        std::vector<std::string> setClauses;

        if (state.caption.has_value())
            setClauses.push_back("caption = :caption");
        if (state.description.has_value())
            setClauses.push_back("description = :description");
        if (state.weight.has_value())
            setClauses.push_back("weight = :weight");
        if (state.orderNumber.has_value())
            setClauses.push_back("orderNumber = :orderNumber");
        if (state.isArchive.has_value())
            setClauses.push_back("isArchive = :isArchive");
        if (state.isQueue.has_value())
            setClauses.push_back("isQueue = :isQueue");

        if (setClauses.empty())
        {
            LOG_WARN << "update: нет полей для обновления";
            return false;
        }

        sql += boost::algorithm::join(setClauses, ", ");
        sql += " WHERE id = :id";

        auto stmt = conn->prepareStatement(sql);

        if (state.caption.has_value())
            stmt->bindString("caption", *state.caption);
        if (state.description.has_value())
            stmt->bindString("description", *state.description);
        if (state.weight.has_value())
            stmt->bindInt64("weight", *state.weight);
        if (state.orderNumber.has_value())
            stmt->bindInt64("orderNumber", *state.orderNumber);
        if (state.isArchive.has_value())
            stmt->bindInt64("isArchive", *state.isArchive ? 1 : 0);
        if (state.isQueue.has_value())
            stmt->bindInt64("isQueue", *state.isQueue ? 1 : 0);

        stmt->bindInt64("id", *state.id);

        int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка обновления состояния: " << e.what();
        return false;
    }
}

bool SqliteStateRepository::remove(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "remove: неверный идентификатор " << id;
        return false;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement("DELETE FROM State WHERE id = :id");
        stmt->bindInt64("id", id);

        int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка удаления состояния: " << e.what();
        return false;
    }
}

std::vector<dto::State> SqliteStateRepository::findByWorkflowId(int64_t workflowId)
{
    std::vector<dto::State> states;

    if (workflowId <= 0)
    {
        LOG_WARN << "findByWorkflowId: неверный workflowId " << workflowId;
        return states;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, workflowId, caption, description, weight, "
            "orderNumber, isArchive, isQueue "
            "FROM State WHERE workflowId = :workflowId ORDER BY orderNumber"
        );

        stmt->bindInt64("workflowId", workflowId);
        auto rs = stmt->executeQuery();

        while (rs->next())
        {
            states.push_back(mapRowToState(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения состояний по workflowId: " << e.what();
        throw;
    }

    return states;
}

bool SqliteStateRepository::isUsedByItems(int64_t stateId)
{
    if (stateId <= 0)
        return false;

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT 1 FROM Item WHERE stateId = :stateId LIMIT 1"
        );

        stmt->bindInt64("stateId", stateId);
        auto rs = stmt->executeQuery();

        return rs->next();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка проверки использования состояния: " << e.what();
        return false;
    }
}

std::shared_ptr<db::IConnection> SqliteStateRepository::connection() const
{
    return m_database->connection();
}

} // namespace repositories
} // namespace server
