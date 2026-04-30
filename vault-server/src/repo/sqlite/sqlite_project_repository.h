#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "repo/project_repository.h"

namespace db
{
class IDatabase;
class IConnection;
class IResultSet;
}

namespace server
{
namespace repositories
{

class SqliteProjectRepository final : public IProjectRepository
{
public:
    explicit SqliteProjectRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteProjectRepository() override = default;

    SqliteProjectRepository(const SqliteProjectRepository&) = delete;
    SqliteProjectRepository& operator=(const SqliteProjectRepository&) = delete;
    SqliteProjectRepository(SqliteProjectRepository&&) = delete;
    SqliteProjectRepository& operator=(SqliteProjectRepository&&) = delete;

    std::pair<std::vector<dto::Project>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> parentId = std::nullopt,
        std::optional<bool> isArchive = std::nullopt,
        const std::string& searchCaption = ""
    ) override;

    std::optional<dto::Project> findById(int64_t id) override;
    int64_t create(const dto::Project& project) override;
    bool update(const dto::Project& project) override;
    bool archive(int64_t id) override;
    bool restore(int64_t id) override;
    bool exists(int64_t id) override;
    int64_t childCount(int64_t parentId) override;

private:
    std::shared_ptr<db::IConnection> connection() const;
    dto::Project mapRowToProject(db::IResultSet& rs) const;

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
