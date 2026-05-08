#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/item_type.h"

#include "repo/item_type_repository.h"

namespace server
{
namespace tests
{

class MockItemTypeRepository : public repositories::IItemTypeRepository
{
public:
    void setFindAllResult(std::pair<std::vector<dto::ItemType>, int64_t> result)
    {
        m_findAllResult = std::move(result);
        m_findAllCallback = [this](
                                int,
                                int,
                                std::optional<int64_t>,
                                std::optional<std::string>,
                                const std::string&
                            )
        {
            return m_findAllResult;
        };
    }

    void setFindAllCallback(
        std::function<std::pair<std::vector<dto::ItemType>, int64_t>(
            int, int, std::optional<int64_t>, std::optional<std::string>, const std::string&
        )>
            callback
    )
    {
        m_findAllCallback = std::move(callback);
    }

    void setFindByIdResult(std::optional<dto::ItemType> itemType)
    {
        m_findByIdResult = std::move(itemType);
    }

    void setCreateResult(int64_t id)
    {
        m_createResult = id;
    }

    void setUpdateResult(bool result)
    {
        m_updateResult = result;
    }

    void setRemoveResult(bool result)
    {
        m_removeResult = result;
    }

    void setExistsResult(bool result)
    {
        m_existsResult = result;
    }

    void setFindByWorkflowResult(std::vector<dto::ItemType> itemTypes)
    {
        m_findByWorkflowResult = std::move(itemTypes);
    }

    void setUsageCountResult(int64_t count)
    {
        m_usageCountResult = count;
    }

    // Реализация интерфейса
    std::pair<std::vector<dto::ItemType>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId,
        std::optional<std::string> kind,
        const std::string& captionSearch
    ) override
    {
        m_lastFindAllPage = page;
        m_lastFindAllPageSize = pageSize;
        m_lastFindAllWorkflowId = workflowId;
        m_lastFindAllKind = kind;
        m_lastFindAllCaptionSearch = captionSearch;
        m_findAllCallCount++;

        if (m_findAllCallback)
        {
            return m_findAllCallback(page, pageSize, workflowId, kind, captionSearch);
        }
        return m_findAllResult;
    }

    std::optional<dto::ItemType> findById(int64_t id) override
    {
        m_lastFindById = id;
        m_findByIdCallCount++;
        return m_findByIdResult;
    }

    int64_t create(const dto::ItemType& itemType) override
    {
        m_lastCreated = itemType;
        m_createCallCount++;
        return m_createResult;
    }

    bool update(const dto::ItemType& itemType) override
    {
        m_lastUpdated = itemType;
        m_updateCallCount++;
        return m_updateResult;
    }

    bool remove(int64_t id) override
    {
        m_lastRemovedId = id;
        m_removeCallCount++;
        return m_removeResult;
    }

    bool exists(int64_t id) override
    {
        m_lastExistsId = id;
        m_existsCallCount++;
        return m_existsResult;
    }

    std::vector<dto::ItemType> findByWorkflow(int64_t workflowId) override
    {
        m_lastFindByWorkflowId = workflowId;
        m_findByWorkflowCallCount++;
        return m_findByWorkflowResult;
    }

    int64_t usageCount(int64_t id) override
    {
        m_lastUsageCountId = id;
        m_usageCountCallCount++;
        return m_usageCountResult;
    }

    // Методы для проверки вызовов
    int getFindAllCallCount() const { return m_findAllCallCount; }
    int getFindByIdCallCount() const { return m_findByIdCallCount; }
    int getCreateCallCount() const { return m_createCallCount; }
    int getUpdateCallCount() const { return m_updateCallCount; }
    int getRemoveCallCount() const { return m_removeCallCount; }
    int getExistsCallCount() const { return m_existsCallCount; }
    int getFindByWorkflowCallCount() const { return m_findByWorkflowCallCount; }
    int getUsageCountCallCount() const { return m_usageCountCallCount; }

    int getLastFindAllPage() const { return m_lastFindAllPage; }
    int getLastFindAllPageSize() const { return m_lastFindAllPageSize; }
    int64_t getLastFindById() const { return m_lastFindById; }
    const dto::ItemType& getLastCreated() const { return m_lastCreated; }
    const dto::ItemType& getLastUpdated() const { return m_lastUpdated; }
    int64_t getLastRemovedId() const { return m_lastRemovedId; }
    int64_t getLastFindByWorkflowId() const { return m_lastFindByWorkflowId; }
    int64_t getLastUsageCountId() const { return m_lastUsageCountId; }

    void reset()
    {
        m_findAllCallCount = 0;
        m_findByIdCallCount = 0;
        m_createCallCount = 0;
        m_updateCallCount = 0;
        m_removeCallCount = 0;
        m_existsCallCount = 0;
        m_findByWorkflowCallCount = 0;
        m_usageCountCallCount = 0;
        m_findAllCallback = nullptr;
    }

private:
    std::pair<std::vector<dto::ItemType>, int64_t> m_findAllResult;
    std::optional<dto::ItemType> m_findByIdResult;
    int64_t m_createResult = 0;
    bool m_updateResult = false;
    bool m_removeResult = false;
    bool m_existsResult = false;
    std::vector<dto::ItemType> m_findByWorkflowResult;
    int64_t m_usageCountResult = 0;

    std::function<std::pair<std::vector<dto::ItemType>, int64_t>(
        int, int, std::optional<int64_t>, std::optional<std::string>, const std::string&
    )>
        m_findAllCallback;

    int m_findAllCallCount = 0;
    int m_findByIdCallCount = 0;
    int m_createCallCount = 0;
    int m_updateCallCount = 0;
    int m_removeCallCount = 0;
    int m_existsCallCount = 0;
    int m_findByWorkflowCallCount = 0;
    int m_usageCountCallCount = 0;

    int m_lastFindAllPage = 0;
    int m_lastFindAllPageSize = 0;
    std::optional<int64_t> m_lastFindAllWorkflowId;
    std::optional<std::string> m_lastFindAllKind;
    std::string m_lastFindAllCaptionSearch;
    int64_t m_lastFindById = 0;
    dto::ItemType m_lastCreated;
    dto::ItemType m_lastUpdated;
    int64_t m_lastRemovedId = 0;
    int64_t m_lastExistsId = 0;
    int64_t m_lastFindByWorkflowId = 0;
    int64_t m_lastUsageCountId = 0;
};

} // namespace tests
} // namespace server
