#pragma once

#include <memory>

#include "../workflow_repository.h"

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
 * @brief SQLite-реализация репозитория рабочих процессов.
 */
class SqliteWorkflowRepository final : public IWorkflowRepository
{
public:
    explicit SqliteWorkflowRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteWorkflowRepository() override = default;

    std::pair<std::vector<dto::Workflow>, int64_t> findAll(
        int page,
        int pageSize
    ) override;
    std::optional<dto::Workflow> findById(int64_t id) override;
    int64_t create(const dto::Workflow& workflow) override;
    bool update(const dto::Workflow& workflow) override;
    bool remove(int64_t id) override;
    bool existsByCaption(const std::string& caption) override;

    std::shared_ptr<db::IConnection> connection() const;

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
