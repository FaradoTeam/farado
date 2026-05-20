#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "repo/phase_repository.h"

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

class SqlitePhaseRepository final : public IPhaseRepository
{
public:
    explicit SqlitePhaseRepository(std::shared_ptr<db::IDatabase> database);
    ~SqlitePhaseRepository() override = default;

    std::pair<std::vector<dto::Phase>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> projectId = std::nullopt,
        std::optional<bool> isArchive = std::nullopt
    ) override;

    std::optional<dto::Phase> findById(int64_t id) override;
    int64_t create(const dto::Phase& phase) override;
    bool update(const dto::Phase& phase) override;
    bool archive(int64_t id) override;
    bool restore(int64_t id) override;
    bool exists(int64_t id) override;
    std::vector<dto::Phase> findByProject(
        int64_t projectId,
        bool includeArchived = false
    ) override;

private:
    std::shared_ptr<db::IConnection> connection() const;
    dto::Phase mapRowToPhase(db::IResultSet& rs) const;

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
