#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/phase.h"

#include "repo/phase_repository.h"

namespace server
{
namespace tests
{

class MockPhaseRepository : public repositories::IPhaseRepository
{
public:
    void setFindAllResult(std::pair<std::vector<dto::Phase>, int64_t> result)
    {
        m_findAllResult = std::move(result);
        m_findAllCallback = [this](int, int, std::optional<int64_t>, std::optional<bool>)
        {
            return m_findAllResult;
        };
    }

    void setFindAllCallback(
        std::function<std::pair<std::vector<dto::Phase>, int64_t>(
            int, int, std::optional<int64_t>, std::optional<bool>
        )>
            callback
    )
    {
        m_findAllCallback = std::move(callback);
    }

    void setFindByIdResult(std::optional<dto::Phase> phase)
    {
        m_findByIdResult = std::move(phase);
    }

    void setCreateResult(int64_t id)
    {
        m_createResult = id;
    }

    void setUpdateResult(bool result)
    {
        m_updateResult = result;
    }

    void setArchiveResult(bool result)
    {
        m_archiveResult = result;
    }

    void setRestoreResult(bool result)
    {
        m_restoreResult = result;
    }

    void setExistsResult(bool result)
    {
        m_existsResult = result;
    }

    void setFindByProjectResult(std::vector<dto::Phase> phases)
    {
        m_findByProjectResult = std::move(phases);
    }

    // Реализация интерфейса
    std::pair<std::vector<dto::Phase>, int64_t> findAll(
        int page, int pageSize,
        std::optional<int64_t> projectId,
        std::optional<bool> isArchive
    ) override
    {
        m_lastFindAllPage = page;
        m_lastFindAllPageSize = pageSize;
        m_lastFindAllProjectId = projectId;
        m_lastFindAllIsArchive = isArchive;
        m_findAllCallCount++;

        if (m_findAllCallback)
        {
            return m_findAllCallback(page, pageSize, projectId, isArchive);
        }
        return m_findAllResult;
    }

    std::optional<dto::Phase> findById(int64_t id) override
    {
        m_lastFindById = id;
        m_findByIdCallCount++;
        return m_findByIdResult;
    }

    int64_t create(const dto::Phase& phase) override
    {
        m_lastCreatedPhase = phase;
        m_createCallCount++;
        return m_createResult;
    }

    bool update(const dto::Phase& phase) override
    {
        m_lastUpdatedPhase = phase;
        m_updateCallCount++;
        return m_updateResult;
    }

    bool archive(int64_t id) override
    {
        m_lastArchivedId = id;
        m_archiveCallCount++;
        return m_archiveResult;
    }

    bool restore(int64_t id) override
    {
        m_lastRestoredId = id;
        m_restoreCallCount++;
        return m_restoreResult;
    }

    bool exists(int64_t id) override
    {
        m_lastExistsId = id;
        m_existsCallCount++;
        return m_existsResult;
    }

    std::vector<dto::Phase> findByProject(int64_t projectId, bool includeArchived) override
    {
        m_lastFindByProjectId = projectId;
        m_lastIncludeArchived = includeArchived;
        m_findByProjectCallCount++;
        return m_findByProjectResult;
    }

    // Методы для проверки вызовов
    int getFindAllCallCount() const { return m_findAllCallCount; }
    int getFindByIdCallCount() const { return m_findByIdCallCount; }
    int getCreateCallCount() const { return m_createCallCount; }
    int getUpdateCallCount() const { return m_updateCallCount; }
    int getArchiveCallCount() const { return m_archiveCallCount; }
    int getRestoreCallCount() const { return m_restoreCallCount; }
    int getExistsCallCount() const { return m_existsCallCount; }

    int getLastFindAllPage() const { return m_lastFindAllPage; }
    int getLastFindAllPageSize() const { return m_lastFindAllPageSize; }
    std::optional<int64_t> getLastFindAllProjectId() const { return m_lastFindAllProjectId; }
    int64_t getLastFindById() const { return m_lastFindById; }
    const dto::Phase& getLastCreatedPhase() const { return m_lastCreatedPhase; }
    const dto::Phase& getLastUpdatedPhase() const { return m_lastUpdatedPhase; }
    int64_t getLastArchivedId() const { return m_lastArchivedId; }
    int64_t getLastRestoredId() const { return m_lastRestoredId; }

    void reset()
    {
        m_findAllCallCount = 0;
        m_findByIdCallCount = 0;
        m_createCallCount = 0;
        m_updateCallCount = 0;
        m_archiveCallCount = 0;
        m_restoreCallCount = 0;
        m_existsCallCount = 0;
        m_findByProjectCallCount = 0;
        m_findAllCallback = nullptr;
    }

private:
    std::pair<std::vector<dto::Phase>, int64_t> m_findAllResult;
    std::optional<dto::Phase> m_findByIdResult;
    int64_t m_createResult = 0;
    bool m_updateResult = false;
    bool m_archiveResult = false;
    bool m_restoreResult = false;
    bool m_existsResult = false;
    std::vector<dto::Phase> m_findByProjectResult;

    std::function<std::pair<std::vector<dto::Phase>, int64_t>(
        int, int, std::optional<int64_t>, std::optional<bool>
    )>
        m_findAllCallback;

    int m_findAllCallCount = 0;
    int m_findByIdCallCount = 0;
    int m_createCallCount = 0;
    int m_updateCallCount = 0;
    int m_archiveCallCount = 0;
    int m_restoreCallCount = 0;
    int m_existsCallCount = 0;
    int m_findByProjectCallCount = 0;

    int m_lastFindAllPage = 0;
    int m_lastFindAllPageSize = 0;
    std::optional<int64_t> m_lastFindAllProjectId;
    std::optional<bool> m_lastFindAllIsArchive;
    int64_t m_lastFindById = 0;
    dto::Phase m_lastCreatedPhase;
    dto::Phase m_lastUpdatedPhase;
    int64_t m_lastArchivedId = 0;
    int64_t m_lastRestoredId = 0;
    int64_t m_lastExistsId = 0;
    int64_t m_lastFindByProjectId = 0;
    bool m_lastIncludeArchived = false;
};

} // namespace tests
} // namespace server
