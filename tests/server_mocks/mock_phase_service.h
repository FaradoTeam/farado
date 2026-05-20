#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/phase.h"

#include "logic/iphase_service.h"

namespace server
{
namespace tests
{

class MockPhaseService : public services::IPhaseService
{
public:
    using PhasesPage = services::PhasesPage;

    // Настройка результатов
    void setGetPhasesResult(const PhasesPage& result)
    {
        m_getPhasesResult = result;
    }

    void setGetPhaseResult(std::optional<dto::Phase> phase)
    {
        m_getPhaseResult = std::move(phase);
    }

    void setCreatePhaseResult(std::optional<dto::Phase> phase)
    {
        m_createPhaseResult = std::move(phase);
    }

    void setUpdatePhaseResult(std::optional<dto::Phase> phase)
    {
        m_updatePhaseResult = std::move(phase);
    }

    void setArchivePhaseResult(bool result)
    {
        m_archivePhaseResult = result;
    }

    void setRestorePhaseResult(bool result)
    {
        m_restorePhaseResult = result;
    }

    // Реализация интерфейса
    PhasesPage phases(
        int page,
        int pageSize,
        std::optional<int64_t> projectId,
        std::optional<bool> isArchive
    ) override
    {
        m_lastGetPhasesPage = page;
        m_lastGetPhasesPageSize = pageSize;
        m_lastGetPhasesProjectId = projectId;
        m_lastGetPhasesIsArchive = isArchive;
        m_getPhasesCallCount++;
        return m_getPhasesResult;
    }

    std::optional<dto::Phase> phase(int64_t id) override
    {
        m_lastGetPhaseId = id;
        m_getPhaseCallCount++;
        return m_getPhaseResult;
    }

    std::optional<dto::Phase> createPhase(const dto::Phase& phase) override
    {
        m_lastCreatedPhase = phase;
        m_createPhaseCallCount++;
        return m_createPhaseResult;
    }

    std::optional<dto::Phase> updatePhase(const dto::Phase& phase) override
    {
        m_lastUpdatedPhase = phase;
        m_updatePhaseCallCount++;
        return m_updatePhaseResult;
    }

    bool archivePhase(int64_t id) override
    {
        m_lastArchivedPhaseId = id;
        m_archivePhaseCallCount++;
        return m_archivePhaseResult;
    }

    bool restorePhase(int64_t id) override
    {
        m_lastRestoredPhaseId = id;
        m_restorePhaseCallCount++;
        return m_restorePhaseResult;
    }

    // Методы для проверки вызовов
    int getPhasesCallCount() const { return m_getPhasesCallCount; }
    int getPhaseCallCount() const { return m_getPhaseCallCount; }
    int createPhaseCallCount() const { return m_createPhaseCallCount; }
    int updatePhaseCallCount() const { return m_updatePhaseCallCount; }
    int archivePhaseCallCount() const { return m_archivePhaseCallCount; }
    int restorePhaseCallCount() const { return m_restorePhaseCallCount; }

    int getLastGetPhasesPage() const { return m_lastGetPhasesPage; }
    int getLastGetPhasesPageSize() const { return m_lastGetPhasesPageSize; }
    std::optional<int64_t> getLastGetPhasesProjectId() const { return m_lastGetPhasesProjectId; }
    int64_t getLastGetPhaseId() const { return m_lastGetPhaseId; }
    const dto::Phase& getLastCreatedPhase() const { return m_lastCreatedPhase; }
    const dto::Phase& getLastUpdatedPhase() const { return m_lastUpdatedPhase; }
    int64_t getLastArchivedPhaseId() const { return m_lastArchivedPhaseId; }
    int64_t getLastRestoredPhaseId() const { return m_lastRestoredPhaseId; }

    void reset()
    {
        m_getPhasesCallCount = 0;
        m_getPhaseCallCount = 0;
        m_createPhaseCallCount = 0;
        m_updatePhaseCallCount = 0;
        m_archivePhaseCallCount = 0;
        m_restorePhaseCallCount = 0;

        m_lastGetPhasesPage = 0;
        m_lastGetPhasesPageSize = 0;
        m_lastGetPhasesProjectId = std::nullopt;
        m_lastGetPhasesIsArchive = std::nullopt;
        m_lastGetPhaseId = 0;
        m_lastCreatedPhase = dto::Phase{};
        m_lastUpdatedPhase = dto::Phase{};
        m_lastArchivedPhaseId = 0;
        m_lastRestoredPhaseId = 0;
    }

private:
    PhasesPage m_getPhasesResult;
    std::optional<dto::Phase> m_getPhaseResult;
    std::optional<dto::Phase> m_createPhaseResult;
    std::optional<dto::Phase> m_updatePhaseResult;
    bool m_archivePhaseResult = false;
    bool m_restorePhaseResult = false;

    int m_getPhasesCallCount = 0;
    int m_getPhaseCallCount = 0;
    int m_createPhaseCallCount = 0;
    int m_updatePhaseCallCount = 0;
    int m_archivePhaseCallCount = 0;
    int m_restorePhaseCallCount = 0;

    int m_lastGetPhasesPage = 0;
    int m_lastGetPhasesPageSize = 0;
    std::optional<int64_t> m_lastGetPhasesProjectId;
    std::optional<bool> m_lastGetPhasesIsArchive;
    int64_t m_lastGetPhaseId = 0;
    dto::Phase m_lastCreatedPhase;
    dto::Phase m_lastUpdatedPhase;
    int64_t m_lastArchivedPhaseId = 0;
    int64_t m_lastRestoredPhaseId = 0;
};

} // namespace tests
} // namespace server
