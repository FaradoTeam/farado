#pragma once

#include <memory>

#include "../state_repository.h"

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
 * @brief SQLite-реализация репозитория состояний.
 */
class SqliteStateRepository final : public IStateRepository
{
public:
    explicit SqliteStateRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteStateRepository() override = default;

    std::pair<std::vector<dto::State>, int64_t> findAll(
        int page, int pageSize,
        std::optional<int64_t> workflowId = std::nullopt
    ) override;
    std::optional<dto::State> findById(int64_t id) override;
    int64_t create(const dto::State& state) override;
    bool update(const dto::State& state) override;
    bool remove(int64_t id) override;
    std::vector<dto::State> findByWorkflowId(int64_t workflowId) override;
    bool isUsedByItems(int64_t stateId) override;

    std::shared_ptr<db::IConnection> connection() const;

private:
    /**
     * @brief Преобразует строку результата в объект State.
     */
    static dto::State mapRowToState(db::IResultSet& rs);

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
