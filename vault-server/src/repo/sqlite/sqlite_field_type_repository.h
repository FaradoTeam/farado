#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "repo/field_type_repository.h"

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
 * @brief SQLite реализация репозитория для работы с типами полей.
 */
class SqliteFieldTypeRepository final : public IFieldTypeRepository
{
public:
    explicit SqliteFieldTypeRepository(std::shared_ptr<db::IDatabase> database);
    ~SqliteFieldTypeRepository() override = default;

    SqliteFieldTypeRepository(const SqliteFieldTypeRepository&) = delete;
    SqliteFieldTypeRepository& operator=(const SqliteFieldTypeRepository&) = delete;
    SqliteFieldTypeRepository(SqliteFieldTypeRepository&&) = delete;
    SqliteFieldTypeRepository& operator=(SqliteFieldTypeRepository&&) = delete;

    std::pair<std::vector<dto::FieldType>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> itemTypeId = std::nullopt,
        std::optional<std::string> valueType = std::nullopt,
        const std::string& captionSearch = ""
    ) override;

    std::optional<dto::FieldType> findById(int64_t id) override;
    int64_t create(const dto::FieldType& fieldType) override;
    bool update(const dto::FieldType& fieldType) override;
    bool remove(int64_t id) override;
    bool exists(int64_t id) override;
    std::vector<dto::FieldType> findByItemType(int64_t itemTypeId) override;
    int64_t usageCount(int64_t id) override;

    std::shared_ptr<db::IConnection> connection() const;

private:
    std::shared_ptr<db::IDatabase> m_database;
};

} // namespace repositories
} // namespace server
