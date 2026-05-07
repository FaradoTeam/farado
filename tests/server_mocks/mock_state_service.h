#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/state.h"

#include "logic/istate_service.h"

namespace server
{
namespace tests
{

class MockStateService : public services::IStateService
{
public:
    using StatesPage = services::StatesPage;
    using StateResult = services::StateResult;

    // Настройка результатов
    void setStatesResult(const StatesPage& result)
    {
        m_statesResult = result;
    }

    void setStateResult(std::optional<dto::State> state)
    {
        m_stateResult = std::move(state);
    }

    void setCreateStateResult(std::optional<dto::State> state)
    {
        m_createStateResult = std::move(state);
    }

    void setUpdateStateResult(std::optional<dto::State> state)
    {
        m_updateStateResult = std::move(state);
    }

    void setDeleteStateResult(const StateResult& result)
    {
        m_deleteStateResult = result;
    }

    void setWorkflowStatesResult(const std::vector<dto::State>& states)
    {
        m_workflowStatesResult = states;
    }

    void setCanDeleteStateResult(bool result)
    {
        m_canDeleteStateResult = result;
    }

    // Реализация интерфейса
    StatesPage states(
        int page, int pageSize,
        std::optional<int64_t> workflowId = std::nullopt
    ) override
    {
        m_lastStatesPage = page;
        m_lastStatesPageSize = pageSize;
        m_lastStatesWorkflowId = workflowId;
        m_statesCallCount++;
        return m_statesResult;
    }

    std::optional<dto::State> state(int64_t id) override
    {
        m_lastStateId = id;
        m_stateCallCount++;
        return m_stateResult;
    }

    std::optional<dto::State> createState(const dto::State& state) override
    {
        m_lastCreatedState = state;
        m_createStateCallCount++;
        return m_createStateResult;
    }

    std::optional<dto::State> updateState(const dto::State& state) override
    {
        m_lastUpdatedState = state;
        m_updateStateCallCount++;
        return m_updateStateResult;
    }

    StateResult deleteState(int64_t id) override
    {
        m_lastDeletedStateId = id;
        m_deleteStateCallCount++;
        return m_deleteStateResult;
    }

    std::vector<dto::State> getWorkflowStates(int64_t workflowId) override
    {
        m_lastWorkflowStatesId = workflowId;
        m_workflowStatesCallCount++;
        return m_workflowStatesResult;
    }

    bool canDeleteState(int64_t id) override
    {
        m_lastCanDeleteStateId = id;
        m_canDeleteStateCallCount++;
        return m_canDeleteStateResult;
    }

    // Методы для проверки вызовов
    int getStatesCallCount() const { return m_statesCallCount; }
    int getStateCallCount() const { return m_stateCallCount; }
    int getCreateStateCallCount() const { return m_createStateCallCount; }
    int getUpdateStateCallCount() const { return m_updateStateCallCount; }
    int getDeleteStateCallCount() const { return m_deleteStateCallCount; }

    int getLastStatesPage() const { return m_lastStatesPage; }
    int getLastStatesPageSize() const { return m_lastStatesPageSize; }
    std::optional<int64_t> getLastStatesWorkflowId() const { return m_lastStatesWorkflowId; }
    int64_t getLastStateId() const { return m_lastStateId; }
    const dto::State& getLastCreatedState() const { return m_lastCreatedState; }
    const dto::State& getLastUpdatedState() const { return m_lastUpdatedState; }
    int64_t getLastDeletedStateId() const { return m_lastDeletedStateId; }

    void reset()
    {
        m_statesCallCount = 0;
        m_stateCallCount = 0;
        m_createStateCallCount = 0;
        m_updateStateCallCount = 0;
        m_deleteStateCallCount = 0;
        m_workflowStatesCallCount = 0;
        m_canDeleteStateCallCount = 0;
        m_lastStatesPage = 0;
        m_lastStatesPageSize = 0;
        m_lastStatesWorkflowId = std::nullopt;
        m_lastStateId = 0;
        m_lastCreatedState = dto::State {};
        m_lastUpdatedState = dto::State {};
        m_lastDeletedStateId = 0;
        m_lastWorkflowStatesId = 0;
    }

private:
    StatesPage m_statesResult;
    std::optional<dto::State> m_stateResult;
    std::optional<dto::State> m_createStateResult;
    std::optional<dto::State> m_updateStateResult;
    StateResult m_deleteStateResult;
    std::vector<dto::State> m_workflowStatesResult;
    bool m_canDeleteStateResult = false;

    int m_statesCallCount = 0;
    int m_stateCallCount = 0;
    int m_createStateCallCount = 0;
    int m_updateStateCallCount = 0;
    int m_deleteStateCallCount = 0;
    int m_workflowStatesCallCount = 0;
    int m_canDeleteStateCallCount = 0;

    int m_lastStatesPage = 0;
    int m_lastStatesPageSize = 0;
    std::optional<int64_t> m_lastStatesWorkflowId;
    int64_t m_lastStateId = 0;
    dto::State m_lastCreatedState;
    dto::State m_lastUpdatedState;
    int64_t m_lastDeletedStateId = 0;
    int64_t m_lastWorkflowStatesId = 0;
    int64_t m_lastCanDeleteStateId = 0;
};

} // namespace tests
} // namespace server
