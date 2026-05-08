// vault-server/src/repo/sqlite/sqlite_item_type_repository.cpp

#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "common/helpers/string_helper.h"
#include "common/log/log.h"

#include "storage/idatabase.h"

#include "sqlite_item_type_repository.h"

namespace
{
dto::ItemType mapRowToItemType(db::IResultSet& rs) const
{
    dto::ItemType itemType;

    itemType.id = rs.valueInt64("id");
    itemType.workflowId = rs.valueInt64("workflowId");

    if (!rs.isNull("defaultStateId"))
        itemType.defaultStateId = rs.valueInt64("defaultStateId");

    itemType.caption = rs.valueString("caption");
    itemType.kind = rs.valueString("kind");

    if (!rs.isNull("defaultContent"))
        itemType.defaultContent = rs.valueString("defaultContent");

    return itemType;
}
} // namespace

namespace server
{
namespace repositories
{

SqliteItemTypeRepository::SqliteItemTypeRepository(
    std::shared_ptr<db::IDatabase> database
)
    : m_database(std::move(database))
{
    if (!m_database)
    {
        throw std::runtime_error("SqliteItemTypeRepository: database is null");
    }
}

std::pair<std::vector<dto::ItemType>, int64_t> SqliteItemTypeRepository::findAll(
    int page,
    int pageSize,
    std::optional<int64_t> workflowId,
    std::optional<std::string> kind,
    const std::string& captionSearch
)
{
    std::vector<dto::ItemType> itemTypes;
    int64_t totalCount = 0;

    try
    {
        auto conn = connection();

        // Строим динамический SQL с условиями фильтрации
        std::string whereClause = "WHERE 1=1";

        // Фильтр по рабочему процессу
        if (workflowId.has_value())
        {
            whereClause += " AND workflowId = :workflowId";
        }

        // Фильтр по виду элемента
        if (kind.has_value())
        {
            whereClause += " AND kind = :kind";
        }

        // Поиск по названию
        if (!captionSearch.empty())
        {
            whereClause += " AND caption LIKE '%' || :caption || '%'";
        }

        // 1. Получаем общее количество
        auto countStmt = conn->prepareStatement(
            "SELECT COUNT(*) FROM ItemType " + whereClause
        );

        if (workflowId.has_value())
            countStmt->bindInt64("workflowId", workflowId.value());

        if (kind.has_value())
            countStmt->bindString("kind", kind.value());

        if (!captionSearch.empty())
            countStmt->bindString("caption", captionSearch);

        auto countRs = countStmt->executeQuery();
        if (countRs->next())
        {
            totalCount = countRs->valueInt64(0);
        }

        if (totalCount == 0 || (page - 1) * pageSize >= totalCount)
        {
            return { itemTypes, totalCount };
        }

        // 2. Получаем страницу с типами элементов
        const int offset = (page - 1) * pageSize;
        std::string selectSql = "SELECT id, workflowId, defaultStateId, caption, "
                                "kind, defaultContent "
                                "FROM ItemType "
            + whereClause + " ORDER BY kind, caption LIMIT :limit OFFSET :offset";

        auto stmt = conn->prepareStatement(selectSql);

        if (workflowId.has_value())
            stmt->bindInt64("workflowId", workflowId.value());

        if (kind.has_value())
            stmt->bindString("kind", kind.value());

        if (!captionSearch.empty())
            stmt->bindString("caption", captionSearch);

        stmt->bindInt64("limit", pageSize);
        stmt->bindInt64("offset", offset);

        auto rs = stmt->executeQuery();
        while (rs->next())
        {
            itemTypes.push_back(mapRowToItemType(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения списка типов элементов: " << e.what();
        throw;
    }

    return { itemTypes, totalCount };
}

std::optional<dto::ItemType> SqliteItemTypeRepository::findById(int64_t id)
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
            "SELECT id, workflowId, defaultStateId, caption, kind, defaultContent "
            "FROM ItemType WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        if (rs->next())
        {
            return mapRowToItemType(*rs);
        }

        return std::nullopt;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка поиска типа элемента по id: " << e.what();
        throw;
    }
}

int64_t SqliteItemTypeRepository::create(const dto::ItemType& itemType)
{
    // Проверка обязательных полей
    if (!itemType.caption.has_value()
        || itemType.caption->empty()
        || !itemType.workflowId.has_value()
        || !itemType.kind.has_value())
    {
        LOG_WARN << "Создание типа элемента: отсутствуют обязательные поля";
        return 0;
    }

    // Проверка допустимых значений kind
    const std::vector<std::string> validKinds = {
        "issue", "folder", "test-case", "test-cycle", "test-execution", "requirement"
    };
    if (std::find(validKinds.begin(), validKinds.end(), itemType.kind.value())
        == validKinds.end())
    {
        LOG_WARN << "Создание типа элемента: недопустимое значение kind '"
                 << itemType.kind.value() << "'";
        return 0;
    }

    try
    {
        auto conn = connection();

        auto stmt = conn->prepareStatement(
            "INSERT INTO ItemType (workflowId, defaultStateId, caption, "
            "kind, defaultContent) "
            "VALUES (:workflowId, :defaultStateId, :caption, :kind, :defaultContent)"
        );

        // Поле workflowId
        stmt->bindInt64("workflowId", itemType.workflowId.value());

        // Поле defaultStateId
        if (itemType.defaultStateId.has_value())
            stmt->bindInt64("defaultStateId", itemType.defaultStateId.value());
        else
            stmt->bindNull("defaultStateId");

        // Поле caption
        stmt->bindString("caption", itemType.caption.value());

        // Поле kind
        stmt->bindString("kind", itemType.kind.value());

        // Поле defaultContent
        if (itemType.defaultContent.has_value())
            stmt->bindString("defaultContent", itemType.defaultContent.value());
        else
            stmt->bindNull("defaultContent");

        stmt->execute();
        return conn->lastInsertId();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка создания типа элемента: " << e.what();
        throw;
    }
}

bool SqliteItemTypeRepository::update(const dto::ItemType& itemType)
{
    if (!itemType.id.has_value())
    {
        LOG_WARN << "update: отсутствует ID типа элемента";
        return false;
    }

    // Проверка допустимых значений kind, если передаётся
    if (itemType.kind.has_value())
    {
        const std::vector<std::string> validKinds = {
            "issue", "folder", "test-case", "test-cycle", "test-execution", "requirement"
        };
        if (std::find(validKinds.begin(), validKinds.end(), itemType.kind.value())
            == validKinds.end())
        {
            LOG_WARN << "update: недопустимое значение kind '"
                     << itemType.kind.value() << "'";
            return false;
        }
    }

    try
    {
        auto conn = connection();
        std::vector<std::string> setClauses;
        std::string sql = "UPDATE ItemType SET ";

        // Обновляем только переданные поля
        if (itemType.workflowId.has_value())
            setClauses.push_back("workflowId = :workflowId");

        if (itemType.defaultStateId.has_value())
            setClauses.push_back("defaultStateId = :defaultStateId");

        if (itemType.caption.has_value())
            setClauses.push_back("caption = :caption");

        if (itemType.kind.has_value())
            setClauses.push_back("kind = :kind");

        if (itemType.defaultContent.has_value())
            setClauses.push_back("defaultContent = :defaultContent");

        if (setClauses.empty())
        {
            LOG_WARN << "update: нет полей для обновления";
            return false;
        }

        sql += boost::algorithm::join(setClauses, ", ");
        sql += " WHERE id = :id";

        auto stmt = conn->prepareStatement(sql);

        // Биндим параметры
        if (itemType.workflowId.has_value())
            stmt->bindInt64("workflowId", itemType.workflowId.value());

        if (itemType.defaultStateId.has_value())
            stmt->bindInt64("defaultStateId", itemType.defaultStateId.value());

        if (itemType.caption.has_value())
            stmt->bindString("caption", itemType.caption.value());

        if (itemType.kind.has_value())
            stmt->bindString("kind", itemType.kind.value());

        if (itemType.defaultContent.has_value())
            stmt->bindString("defaultContent", itemType.defaultContent.value());

        stmt->bindInt64("id", itemType.id.value());

        const int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка обновления типа элемента: " << e.what();
        return false;
    }
}

bool SqliteItemTypeRepository::remove(int64_t id)
{
    if (id <= 0)
    {
        LOG_WARN << "remove: неверный идентификатор " << id;
        return false;
    }

    try
    {
        // Проверяем, используется ли тип элемента
        const auto count = usageCount(id);
        if (count > 0)
        {
            LOG_WARN << "remove: тип элемента с id=" << id
                     << " используется " << count << " элементами";
            return false;
        }

        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "DELETE FROM ItemType WHERE id = :id"
        );

        stmt->bindInt64("id", id);
        const int64_t affected = stmt->execute();
        return affected > 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка удаления типа элемента: " << e.what();
        return false;
    }
}

bool SqliteItemTypeRepository::exists(int64_t id)
{
    if (id <= 0)
        return false;

    try
    {
        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "SELECT 1 FROM ItemType WHERE id = :id LIMIT 1"
        );

        stmt->bindInt64("id", id);
        auto rs = stmt->executeQuery();

        return rs->next();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка проверки существования типа элемента: " << e.what();
        return false;
    }
}

std::vector<dto::ItemType> SqliteItemTypeRepository::findByWorkflow(int64_t workflowId)
{
    std::vector<dto::ItemType> itemTypes;

    if (workflowId <= 0)
    {
        LOG_WARN << "findByWorkflow: неверный идентификатор рабочего процесса " << workflowId;
        return itemTypes;
    }

    try
    {
        auto conn = connection();
        auto stmt = conn->prepareStatement(
            "SELECT id, workflowId, defaultStateId, caption, kind, defaultContent "
            "FROM ItemType WHERE workflowId = :workflowId ORDER BY caption"
        );

        stmt->bindInt64("workflowId", workflowId);
        auto rs = stmt->executeQuery();

        while (rs->next())
        {
            itemTypes.push_back(mapRowToItemType(*rs));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка получения типов элементов для рабочего процесса: " << e.what();
    }

    return itemTypes;
}

int64_t SqliteItemTypeRepository::usageCount(int64_t id)
{
    if (id <= 0)
        return 0;

    try
    {
        auto conn = connection();

        // Проверяем использование в таблицах Item, FieldType, LinkType, RuleItemType
        auto stmt = conn->prepareStatement(
            "SELECT "
            "(SELECT COUNT(*) FROM Item WHERE itemTypeId = :id1) + "
            "(SELECT COUNT(*) FROM FieldType WHERE itemTypeId = :id2) + "
            "(SELECT COUNT(*) FROM LinkType WHERE sourceItemTypeId = :id3 OR destinationItemTypeId = :id4) + "
            "(SELECT COUNT(*) FROM RuleItemType WHERE itemTypeId = :id5) "
            "AS total_count"
        );

        stmt->bindInt64("id1", id);
        stmt->bindInt64("id2", id);
        stmt->bindInt64("id3", id);
        stmt->bindInt64("id4", id);
        stmt->bindInt64("id5", id);

        auto rs = stmt->executeQuery();
        if (rs->next())
        {
            return rs->valueInt64(0);
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка подсчета использования типа элемента: " << e.what();
        return 0;
    }
}

std::shared_ptr<db::IConnection> SqliteItemTypeRepository::connection() const
{
    return m_database->connection();
}

} // namespace repositories
} // namespace server
