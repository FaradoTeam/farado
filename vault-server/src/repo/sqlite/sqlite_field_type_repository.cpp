#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "common/log/log.h"

#include "storage/idatabase.h"

#include "sqlite_field_type_repository.h"

namespace
{
dto::FieldType mapRowToFieldType(db::IResultSet& rs)
{
    dto::FieldType fieldType;

    fieldType.id = rs.valueInt64("id");
    fieldType.itemTypeId = rs.valueInt64("itemTypeId");
    fieldType.caption = rs.valueString("caption");

    if (!rs.isNull("description"))
        fieldType.description = rs.valueString("description");

    fieldType.valueType = rs.valueString("valueType");

    fieldType.isBoardVisible = rs.valueInt64("isBoardVisible") != 0;

    return fieldType;
}
} // namespace

namespace server
{
namespace repositories
{

SqliteFieldTypeRepository::SqliteFieldTypeRepository(
    std::shared_ptr<db::IDatabase> database
)
    : m_database(std::move(database))
{
    if (!m_database)
    {
        throw std::runtime_error("SqliteFieldTypeRepository: database is null");
    }
}

std::pair<std::vector<dto::FieldType>, int64_t> SqliteFieldTypeRepository::findAll(
    int page,
    int pageSize,
    std::optional<int64_t> itemTypeId,
    std::optional<std::string> valueType,
    const std::string& captionSearch
)
{
    std::vector<dto::FieldType> fieldTypes;
    int64_t totalCount = 0;

    try
    {
        auto conn = connection();

        // Строим динамический SQL с условиями фильтрации
        std::string whereClause = "WHERE 1=1";

        // Фильтр по типу элемента
        if (itemTypeId.has_value())
        {
            whereClause += " AND itemTypeId = :itemTypeId";
        }

        // Фильтр по типу значения
        if (valueType.has_value())
        {
            whereClause += " AND valueType = :valueType";
        }

        // Поиск по названию
        if (!captionSearch.empty())
        {
            whereClause += " AND caption LIKE '%' || :caption || '%'";
        }

        // 1. Получаем общее количество
        auto countStmt = conn->prepareStatement(
            "SELECT COUNT(*) FROM FieldType " + whereClause
        );

        if (itemTypeId.has_value())
            countStmt->bindInt64("itemTypeId", itemTypeId.value());

        if (valueType.has_value())
            countStmt->bindString("valueType", valueType.value());

        if (!captionSearch.empty())
            countStmt->bindString("caption", captionSearch);

        auto countRs = countStmt->executeQuery();
        if (countRs->next())
        {
            totalCount = countRs->valueInt64(0);
        }

        if (totalCount == 0 || (page - 1) * pageSize >= totalCount)
        {
            return { fieldTypes, totalCount };
        }

        // 2. Получаем страницу с типами полей
        const int offset = (page - 1) * pageSize;
        auto stmt = conn->prepareStatement(
            std::string()
            + "SELECT id, itemTypeId, caption, description, valueType, isBoardVisible "
            + "FROM FieldType "
            + whereClause
            + " ORDER BY caption LIMIT :limit OFFSET :offset"
        );

        if (itemTypeId.has_value())
            stmt->bindInt64("itemTypeId", itemTypeId.value());

        if (valueType.has_value())
            stmt->bindString("valueType", valueType.value());

        if (!captionSearch.empty())
            stmt->bindString("caption", captionSearch);

        stmt->bindInt64("limit", pageSize);
        stmt->bindInt64("offset", offset);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            fieldTypes.push_back(mapRowToFieldType(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения списка типов полей: " << e.what();
        throw;
    }

    return { fieldTypes, totalCount };
}

std::optional<dto::FieldType> SqliteFieldTypeRepository::findById(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "findById: неверный идентификатор " << id;
        return std::nullopt;
    }

    try
    {
        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, itemTypeId, caption, description, valueType, isBoardVisible "
            "FROM FieldType WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        if (rs->next())
        {
            return mapRowToFieldType(*rs);
        }

        return std::nullopt;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка поиска типа поля по id: " << e.what();
        throw;
    }
}

int64_t SqliteFieldTypeRepository::create(const dto::FieldType& fieldType)
{
    // Проверка обязательных полей
    if (!fieldType.caption.has_value()
        || fieldType.caption->empty()
        || !fieldType.itemTypeId.has_value()
        || !fieldType.valueType.has_value()
        || fieldType.valueType->empty())
    {
        LOG_WARN << "Создание типа поля: отсутствуют обязательные поля";
        return 0;
    }

    // TODO: Перенести проверку допустимых значений valueType
    const std::vector<std::string> validValueTypes = {
        "String", "MarkdownText", "Integer", "Float", "Bool",
        "DateTime", "Date", "Uri", "Select", "SelectMulti",
        "ItemId", "UserId", "ProjectId", "PhaseId",
        "ItemsIds", "UsersIds", "ProjectsIds", "PhasesIds"
    };
    if (validValueTypes.end() == std::find(validValueTypes.begin(), validValueTypes.end(), fieldType.valueType.value()))
    {
        LOG_WARN
            << "Создание типа поля: недопустимое значение valueType '"
            << fieldType.valueType.value() << "'";
        return 0;
    }

    try
    {
        auto conn = connection();

        auto stmt = conn->prepareStatement(
            "INSERT INTO FieldType (itemTypeId, caption, description, "
            "valueType, isBoardVisible) "
            "VALUES (:itemTypeId, :caption, :description, :valueType, :isBoardVisible)"
        );

        // Поле itemTypeId
        stmt->bindInt64("itemTypeId", fieldType.itemTypeId.value());

        // Поле caption
        stmt->bindString("caption", fieldType.caption.value());

        // Поле description
        if (fieldType.description.has_value())
            stmt->bindString("description", fieldType.description.value());
        else
            stmt->bindNull("description");

        // Поле valueType
        stmt->bindString("valueType", fieldType.valueType.value());

        // Поле isBoardVisible
        stmt->bindInt64("isBoardVisible", fieldType.isBoardVisible.value_or(false) ? 1 : 0);

        stmt->execute();
        return conn->lastInsertId();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка создания типа поля: " << e.what();
        throw;
    }
}

bool SqliteFieldTypeRepository::update(const dto::FieldType& fieldType)
{
    if (!fieldType.id.has_value())
    {
        LOG_WARN << "update: отсутствует ID типа поля";
        return false;
    }

    // Проверка допустимых значений valueType, если передаётся
    if (fieldType.valueType.has_value())
    {
        // TODO: Перенести проверку допустимых значений valueType
        const std::vector<std::string> validValueTypes = {
            "String", "MarkdownText", "Integer", "Float", "Bool",
            "DateTime", "Date", "Uri", "Select", "SelectMulti",
            "ItemId", "UserId", "ProjectId", "PhaseId",
            "ItemsIds", "UsersIds", "ProjectsIds", "PhasesIds"
        };
        if (std::find(validValueTypes.begin(), validValueTypes.end(), fieldType.valueType.value()) == validValueTypes.end())
        {
            LOG_WARN
                << "update: недопустимое значение valueType '"
                << fieldType.valueType.value() << "'";
            return false;
        }
    }

    try
    {
        auto conn = connection();
        std::vector<std::string> setClauses;
        std::string sql = "UPDATE FieldType SET ";

        // Обновляем только переданные поля
        if (fieldType.itemTypeId.has_value())
            setClauses.push_back("itemTypeId = :itemTypeId");

        if (fieldType.caption.has_value())
            setClauses.push_back("caption = :caption");

        if (fieldType.description.has_value())
            setClauses.push_back("description = :description");

        if (fieldType.valueType.has_value())
            setClauses.push_back("valueType = :valueType");

        if (fieldType.isBoardVisible.has_value())
            setClauses.push_back("isBoardVisible = :isBoardVisible");

        if (setClauses.empty())
        {
            LOG_WARN << "update: нет полей для обновления";
            return false;
        }

        sql += boost::algorithm::join(setClauses, ", ");
        sql += " WHERE id = :id";

        auto stmt = conn->prepareStatement(sql);

        // Биндим параметры
        if (fieldType.itemTypeId.has_value())
            stmt->bindInt64("itemTypeId", fieldType.itemTypeId.value());

        if (fieldType.caption.has_value())
            stmt->bindString("caption", fieldType.caption.value());

        if (fieldType.description.has_value())
            stmt->bindString("description", fieldType.description.value());

        if (fieldType.valueType.has_value())
            stmt->bindString("valueType", fieldType.valueType.value());

        if (fieldType.isBoardVisible.has_value())
            stmt->bindInt64("isBoardVisible", fieldType.isBoardVisible.value() ? 1 : 0);

        stmt->bindInt64("id", fieldType.id.value());

        const int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка обновления типа поля: " << e.what();
        return false;
    }
}

bool SqliteFieldTypeRepository::remove(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "remove: неверный идентификатор " << id;
        return false;
    }

    try
    {
        // Проверяем, используется ли тип поля
        const auto count = usageCount(id);
        if (count > 0)
        {
            LOG_WARN
                << "remove: тип поля с id=" << id
                << " используется " << count << " записями";
            return false;
        }

        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "DELETE FROM FieldType WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        const int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка удаления типа поля: " << e.what();
        return false;
    }
}

bool SqliteFieldTypeRepository::exists(int64_t id)
{
    if (id <= 0)
        return false;

    try
    {
        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "SELECT 1 FROM FieldType WHERE id = :id LIMIT 1"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        return rs->next();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка проверки существования типа поля: " << e.what();
        return false;
    }
}

std::vector<dto::FieldType> SqliteFieldTypeRepository::findByItemType(int64_t itemTypeId)
{
    std::vector<dto::FieldType> fieldTypes;

    if (itemTypeId <= 0)
    {
        LOG_WARN << "findByItemType: неверный идентификатор типа элемента " << itemTypeId;
        return fieldTypes;
    }

    try
    {
        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, itemTypeId, caption, description, valueType, isBoardVisible "
            "FROM FieldType WHERE itemTypeId = :itemTypeId ORDER BY caption"
        );

        stmt->bindInt64("itemTypeId", itemTypeId);
        auto rs = stmt->executeQuery();

        while (rs->next())
        {
            fieldTypes.push_back(mapRowToFieldType(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения типов полей для типа элемента: " << e.what();
    }

    return fieldTypes;
}

int64_t SqliteFieldTypeRepository::usageCount(int64_t id)
{
    if (id <= 0)
        return 0;

    try
    {
        auto conn = connection();

        // Проверяем использование в таблицах ItemField и FieldTypePossibleValue
        auto stmt = conn->prepareStatement(
            "SELECT "
            "(SELECT COUNT(*) FROM ItemField WHERE fieldTypeId = :id1) + "
            "(SELECT COUNT(*) FROM FieldTypePossibleValue WHERE fieldTypeId = :id2) "
            "AS total_count"
        );

        stmt->bindInt64("id1", id);
        stmt->bindInt64("id2", id);

        auto rs = stmt->executeQuery();
        if (rs->next())
        {
            return rs->valueInt64(0);
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка подсчета использования типа поля: " << e.what();
        return 0;
    }
}

std::shared_ptr<db::IConnection> SqliteFieldTypeRepository::connection() const
{
    return m_database->connection();
}

} // namespace repositories
} // namespace server
