#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "common/log/log.h"

#include "storage/idatabase.h"

#include "sqlite_edge_repository.h"

namespace
{

/**
 * @brief Преобразует строку результата в объект Edge.
 */
dto::Edge mapRowToEdge(db::IResultSet& rs)
{
    dto::Edge edge;
    edge.id = rs.valueInt64("id");
    edge.beginStateId = rs.valueInt64("beginStateId");
    edge.endStateId = rs.valueInt64("endStateId");

    return edge;
}

} // namespace

namespace server
{
namespace repositories
{

SqliteEdgeRepository::SqliteEdgeRepository(std::shared_ptr<db::IDatabase> database)
    : m_database(std::move(database))
{
    if (!m_database)
    {
        throw std::runtime_error("SqliteEdgeRepository: database is null");
    }
}

std::pair<std::vector<dto::Edge>, int64_t>
SqliteEdgeRepository::findAll(
    int page, int pageSize,
    std::optional<int64_t> beginStateId,
    std::optional<int64_t> endStateId
)
{
    std::vector<dto::Edge> edges;
    int64_t totalCount = 0;

    try
    {
        auto conn = m_database->connection();

        // Формируем SQL с учётом фильтров
        std::vector<std::string> whereClauses;
        if (beginStateId.has_value())
            whereClauses.push_back("beginStateId = :beginStateId");
        if (endStateId.has_value())
            whereClauses.push_back("endStateId = :endStateId");

        std::string whereStr;
        if (!whereClauses.empty())
        {
            whereStr = " WHERE " + boost::algorithm::join(whereClauses, " AND ");
        }

        // Получаем общее количество
        std::string countSql = "SELECT COUNT(*) FROM Edge" + whereStr;
        auto countStmt = conn->prepareStatement(countSql);

        if (beginStateId.has_value())
            countStmt->bindInt64("beginStateId", *beginStateId);
        if (endStateId.has_value())
            countStmt->bindInt64("endStateId", *endStateId);

        auto countRs = countStmt->executeQuery();
        if (countRs->next())
        {
            totalCount = countRs->valueInt64(0);
        }

        if (totalCount == 0)
        {
            return { edges, totalCount };
        }

        // Получаем страницу с переходами
        const int offset = (page - 1) * pageSize;
        std::string selectSql = "SELECT e.id, e.beginStateId, e.endStateId "
                                "FROM Edge e"
            + whereStr + " ORDER BY e.id LIMIT :limit OFFSET :offset";

        auto stmt = conn->prepareStatement(selectSql);

        if (beginStateId.has_value())
            stmt->bindInt64("beginStateId", *beginStateId);
        if (endStateId.has_value())
            stmt->bindInt64("endStateId", *endStateId);
        stmt->bindInt64("limit", pageSize);
        stmt->bindInt64("offset", offset);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            edges.push_back(mapRowToEdge(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения списка переходов: " << e.what();
        throw;
    }

    return { edges, totalCount };
}

std::optional<dto::Edge> SqliteEdgeRepository::findById(int64_t id)
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
            "SELECT id, beginStateId, endStateId FROM Edge WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        if (rs->next())
        {
            return mapRowToEdge(*rs);
        }

        return std::nullopt;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка поиска перехода по id: " << e.what();
        throw;
    }
}

int64_t SqliteEdgeRepository::create(const dto::Edge& edge)
{
    if (!edge.beginStateId.has_value() || !edge.endStateId.has_value())
    {
        LOG_WARN << "Создание перехода: отсутствуют обязательные поля";
        return 0;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "INSERT INTO Edge (beginStateId, endStateId) "
            "VALUES (:beginStateId, :endStateId)"
        );

        stmt->bindInt64("beginStateId", *edge.beginStateId);
        stmt->bindInt64("endStateId", *edge.endStateId);

        stmt->execute();
        return conn->lastInsertId();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка создания перехода: " << e.what();
        throw;
    }
}

bool SqliteEdgeRepository::remove(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "remove: неверный идентификатор " << id;
        return false;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement("DELETE FROM Edge WHERE id = :id");
        stmt->bindInt64("id", id);

        int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка удаления перехода: " << e.what();
        return false;
    }
}

std::vector<dto::Edge> SqliteEdgeRepository::findByStateId(int64_t stateId)
{
    std::vector<dto::Edge> edges;

    if (stateId <= 0)
    {
        LOG_WARN << "findByStateId: неверный stateId " << stateId;
        return edges;
    }

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, beginStateId, endStateId FROM Edge "
            "WHERE beginStateId = :stateId OR endStateId = :stateId2 "
            "ORDER BY id"
        );

        stmt->bindInt64("stateId", stateId);
        stmt->bindInt64("stateId2", stateId);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            edges.push_back(mapRowToEdge(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения переходов по stateId: " << e.what();
        throw;
    }

    return edges;
}

bool SqliteEdgeRepository::exists(int64_t beginStateId, int64_t endStateId)
{
    if (beginStateId <= 0 || endStateId <= 0)
        return false;

    try
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "SELECT 1 FROM Edge WHERE beginStateId = :beginStateId "
            "AND endStateId = :endStateId LIMIT 1"
        );

        stmt->bindInt64("beginStateId", beginStateId);
        stmt->bindInt64("endStateId", endStateId);

        auto rs = stmt->executeQuery();
        return rs->next();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка проверки существования перехода: " << e.what();
        return false;
    }
}

std::vector<dto::Edge> SqliteEdgeRepository::findByWorkflowId(int64_t workflowId)
{
    std::vector<dto::Edge> edges;

    if (workflowId <= 0)
    {
        LOG_WARN << "findByWorkflowId: неверный workflowId " << workflowId;
        return edges;
    }

    try
    {
        auto conn = m_database->connection();
        // Получаем переходы, где оба состояния принадлежат одному рабочему процессу
        auto stmt = conn->prepareStatement(
            "SELECT e.id, e.beginStateId, e.endStateId "
            "FROM Edge e "
            "INNER JOIN State s1 ON e.beginStateId = s1.id "
            "INNER JOIN State s2 ON e.endStateId = s2.id "
            "WHERE s1.workflowId = :workflowId AND s2.workflowId = :workflowId2 "
            "ORDER BY e.id"
        );

        stmt->bindInt64("workflowId", workflowId);
        stmt->bindInt64("workflowId2", workflowId);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            edges.push_back(mapRowToEdge(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения переходов по workflowId: " << e.what();
        throw;
    }

    return edges;
}

std::shared_ptr<db::IConnection> SqliteEdgeRepository::connection() const
{
    return m_database->connection();
}

} // namespace repositories
} // namespace server
