#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/item_type.h"

#include "logic/iitem_type_service.h"

namespace server
{
namespace tests
{

class MockItemTypeService : public services::IItemTypeService
{
public:
    using ItemTypesPage = services::ItemTypesPage;

    void setGetItemTypesResult(const ItemTypesPage& result)
    {
        m_getItemTypesResult = result;
    }

    void setGetItemTypeResult(std::optional<dto::ItemType> itemType)
    {
        m_getItemTypeResult = std::move(itemType);
    }

    void setCreateItemTypeResult(std::optional<dto::ItemType> itemType)
    {
        m_createItemTypeResult = std::move(itemType);
    }

    void setUpdateItemTypeResult(std::optional<dto::ItemType> itemType)
    {
        m_updateItemTypeResult = std::move(itemType);
    }

    void setDeleteItemTypeResult(bool result)
    {
        m_deleteItemTypeResult = result;
    }

    void setItemTypesByWorkflowResult(std::vector<dto::ItemType> itemTypes)
    {
        m_itemTypesByWorkflowResult = std::move(itemTypes);
    }

    // Реализация интерфейса
    ItemTypesPage itemTypes(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId,
        std::optional<std::string> kind,
        const std::string& searchCaption
    ) override
    {
        m_lastGetItemTypesPage = page;
        m_lastGetItemTypesPageSize = pageSize;
        m_lastGetItemTypesWorkflowId = workflowId;
        m_lastGetItemTypesKind = kind;
        m_lastGetItemTypesSearch = searchCaption;
        m_getItemTypesCallCount++;
        return m_getItemTypesResult;
    }

    std::optional<dto::ItemType> itemType(int64_t id) override
    {
        m_lastGetItemTypeId = id;
        m_getItemTypeCallCount++;
        return m_getItemTypeResult;
    }

    std::optional<dto::ItemType> createItemType(const dto::ItemType& itemType) override
    {
        m_lastCreatedItemType = itemType;
        m_createItemTypeCallCount++;
        return m_createItemTypeResult;
    }

    std::optional<dto::ItemType> updateItemType(const dto::ItemType& itemType) override
    {
        m_lastUpdatedItemType = itemType;
        m_updateItemTypeCallCount++;
        return m_updateItemTypeResult;
    }

    bool deleteItemType(int64_t id) override
    {
        m_lastDeletedItemTypeId = id;
        m_deleteItemTypeCallCount++;
        return m_deleteItemTypeResult;
    }

    std::vector<dto::ItemType> itemTypesByWorkflow(int64_t workflowId) override
    {
        m_lastItemTypesByWorkflowId = workflowId;
        m_itemTypesByWorkflowCallCount++;
        return m_itemTypesByWorkflowResult;
    }

    // Методы для проверки вызовов
    int getItemTypesCallCount() const { return m_getItemTypesCallCount; }
    int getItemTypeCallCount() const { return m_getItemTypeCallCount; }
    int createItemTypeCallCount() const { return m_createItemTypeCallCount; }
    int updateItemTypeCallCount() const { return m_updateItemTypeCallCount; }
    int deleteItemTypeCallCount() const { return m_deleteItemTypeCallCount; }

    int getLastGetItemTypesPage() const { return m_lastGetItemTypesPage; }
    int64_t getLastGetItemTypeId() const { return m_lastGetItemTypeId; }
    const dto::ItemType& getLastCreatedItemType() const { return m_lastCreatedItemType; }
    const dto::ItemType& getLastUpdatedItemType() const { return m_lastUpdatedItemType; }
    int64_t getLastDeletedItemTypeId() const { return m_lastDeletedItemTypeId; }

    void reset()
    {
        m_getItemTypesCallCount = 0;
        m_getItemTypeCallCount = 0;
        m_createItemTypeCallCount = 0;
        m_updateItemTypeCallCount = 0;
        m_deleteItemTypeCallCount = 0;
        m_itemTypesByWorkflowCallCount = 0;

        m_lastGetItemTypesPage = 0;
        m_lastGetItemTypesPageSize = 0;
        m_lastGetItemTypeId = 0;
        m_lastDeletedItemTypeId = 0;
    }

private:
    ItemTypesPage m_getItemTypesResult;
    std::optional<dto::ItemType> m_getItemTypeResult;
    std::optional<dto::ItemType> m_createItemTypeResult;
    std::optional<dto::ItemType> m_updateItemTypeResult;
    bool m_deleteItemTypeResult = false;
    std::vector<dto::ItemType> m_itemTypesByWorkflowResult;

    int m_getItemTypesCallCount = 0;
    int m_getItemTypeCallCount = 0;
    int m_createItemTypeCallCount = 0;
    int m_updateItemTypeCallCount = 0;
    int m_deleteItemTypeCallCount = 0;
    int m_itemTypesByWorkflowCallCount = 0;

    int m_lastGetItemTypesPage = 0;
    int m_lastGetItemTypesPageSize = 0;
    std::optional<int64_t> m_lastGetItemTypesWorkflowId;
    std::optional<std::string> m_lastGetItemTypesKind;
    std::string m_lastGetItemTypesSearch;
    int64_t m_lastGetItemTypeId = 0;
    dto::ItemType m_lastCreatedItemType;
    dto::ItemType m_lastUpdatedItemType;
    int64_t m_lastDeletedItemTypeId = 0;
    int64_t m_lastItemTypesByWorkflowId = 0;
};

} // namespace tests
} // namespace server
