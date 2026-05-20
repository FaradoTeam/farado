#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "repo/item_type_repository.h"

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

/**
 * @brief SQLite реализация репозитория для работы с типами элементов.
 */
class SqliteItemTypeRepository final : public IItemTypeRepository
{
public:
    explicit SqliteItemTypeRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteItemTypeRepository() override = default;

    std::pair<std::vector<dto::ItemType>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId = std::nullopt,
        std::optional<std::string> kind = std::nullopt,
        const std::string& captionSearch = ""
    ) override;

    std::optional<dto::ItemType> findById(int64_t id) override;
    int64_t create(const dto::ItemType& itemType) override;
    bool update(const dto::ItemType& itemType) override;
    bool remove(int64_t id) override;
    bool exists(int64_t id) override;
    std::vector<dto::ItemType> findByWorkflow(int64_t workflowId) override;
    int64_t usageCount(int64_t id) override;

    std::shared_ptr<db::IConnection> connection() const;

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
