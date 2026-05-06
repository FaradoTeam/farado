#pragma once

#include <memory>

#include "../edge_repository.h"

namespace db
{
class IDatabase;
class IConnection;
}

namespace server
{
namespace repositories
{

/**
 * @brief SQLite-реализация репозитория переходов.
 */
class SqliteEdgeRepository final : public IEdgeRepository
{
public:
    explicit SqliteEdgeRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteEdgeRepository() override = default;

    std::pair<std::vector<dto::Edge>, int64_t> findAll(
        int page, int pageSize,
        std::optional<int64_t> beginStateId = std::nullopt,
        std::optional<int64_t> endStateId = std::nullopt
    ) override;
    std::optional<dto::Edge> findById(int64_t id) override;
    int64_t create(const dto::Edge& edge) override;
    bool remove(int64_t id) override;
    std::vector<dto::Edge> findByStateId(int64_t stateId) override;
    bool exists(int64_t beginStateId, int64_t endStateId) override;
    std::vector<dto::Edge> findByWorkflowId(int64_t workflowId) override;

    std::shared_ptr<db::IConnection> connection() const;

private:
    /**
     * @brief Преобразует строку результата в объект Edge.
     */
    static dto::Edge mapRowToEdge(db::IResultSet& rs);

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
