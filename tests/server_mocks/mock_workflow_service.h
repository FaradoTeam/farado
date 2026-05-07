#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/workflow.h"

#include "logic/iworkflow_service.h"

namespace server
{
namespace tests
{

class MockWorkflowService : public services::IWorkflowService
{
public:
    using WorkflowsPage = services::WorkflowsPage;
    using WorkflowResult = services::WorkflowResult;

    // Настройка результатов
    void setWorkflowsResult(const WorkflowsPage& result)
    {
        m_workflowsResult = result;
    }

    void setWorkflowResult(std::optional<dto::Workflow> workflow)
    {
        m_workflowResult = std::move(workflow);
    }

    void setCreateWorkflowResult(std::optional<dto::Workflow> workflow)
    {
        m_createWorkflowResult = std::move(workflow);
    }

    void setUpdateWorkflowResult(std::optional<dto::Workflow> workflow)
    {
        m_updateWorkflowResult = std::move(workflow);
    }

    void setDeleteWorkflowResult(const WorkflowResult& result)
    {
        m_deleteWorkflowResult = result;
    }

    void setCanDeleteWorkflowResult(bool result)
    {
        m_canDeleteWorkflowResult = result;
    }

    // Реализация интерфейса
    WorkflowsPage workflows(int page, int pageSize) override
    {
        m_lastWorkflowsPage = page;
        m_lastWorkflowsPageSize = pageSize;
        m_workflowsCallCount++;
        return m_workflowsResult;
    }

    std::optional<dto::Workflow> workflow(int64_t id) override
    {
        m_lastWorkflowId = id;
        m_workflowCallCount++;
        return m_workflowResult;
    }

    std::optional<dto::Workflow> createWorkflow(const dto::Workflow& workflow) override
    {
        m_lastCreatedWorkflow = workflow;
        m_createWorkflowCallCount++;
        return m_createWorkflowResult;
    }

    std::optional<dto::Workflow> updateWorkflow(const dto::Workflow& workflow) override
    {
        m_lastUpdatedWorkflow = workflow;
        m_updateWorkflowCallCount++;
        return m_updateWorkflowResult;
    }

    WorkflowResult deleteWorkflow(int64_t id) override
    {
        m_lastDeletedWorkflowId = id;
        m_deleteWorkflowCallCount++;
        return m_deleteWorkflowResult;
    }

    bool canDeleteWorkflow(int64_t id) override
    {
        m_lastCanDeleteWorkflowId = id;
        m_canDeleteWorkflowCallCount++;
        return m_canDeleteWorkflowResult;
    }

    // Методы для проверки вызовов
    int getWorkflowsCallCount() const { return m_workflowsCallCount; }
    int getWorkflowCallCount() const { return m_workflowCallCount; }
    int getCreateWorkflowCallCount() const { return m_createWorkflowCallCount; }
    int getUpdateWorkflowCallCount() const { return m_updateWorkflowCallCount; }
    int getDeleteWorkflowCallCount() const { return m_deleteWorkflowCallCount; }

    int getLastWorkflowsPage() const { return m_lastWorkflowsPage; }
    int getLastWorkflowsPageSize() const { return m_lastWorkflowsPageSize; }
    int64_t getLastWorkflowId() const { return m_lastWorkflowId; }
    const dto::Workflow& getLastCreatedWorkflow() const { return m_lastCreatedWorkflow; }
    const dto::Workflow& getLastUpdatedWorkflow() const { return m_lastUpdatedWorkflow; }
    int64_t getLastDeletedWorkflowId() const { return m_lastDeletedWorkflowId; }

    void reset()
    {
        m_workflowsCallCount = 0;
        m_workflowCallCount = 0;
        m_createWorkflowCallCount = 0;
        m_updateWorkflowCallCount = 0;
        m_deleteWorkflowCallCount = 0;
        m_canDeleteWorkflowCallCount = 0;
        m_lastWorkflowsPage = 0;
        m_lastWorkflowsPageSize = 0;
        m_lastWorkflowId = 0;
        m_lastCreatedWorkflow = dto::Workflow {};
        m_lastUpdatedWorkflow = dto::Workflow {};
        m_lastDeletedWorkflowId = 0;
    }

private:
    WorkflowsPage m_workflowsResult;
    std::optional<dto::Workflow> m_workflowResult;
    std::optional<dto::Workflow> m_createWorkflowResult;
    std::optional<dto::Workflow> m_updateWorkflowResult;
    WorkflowResult m_deleteWorkflowResult;
    bool m_canDeleteWorkflowResult = false;

    int m_workflowsCallCount = 0;
    int m_workflowCallCount = 0;
    int m_createWorkflowCallCount = 0;
    int m_updateWorkflowCallCount = 0;
    int m_deleteWorkflowCallCount = 0;
    int m_canDeleteWorkflowCallCount = 0;

    int m_lastWorkflowsPage = 0;
    int m_lastWorkflowsPageSize = 0;
    int64_t m_lastWorkflowId = 0;
    dto::Workflow m_lastCreatedWorkflow;
    dto::Workflow m_lastUpdatedWorkflow;
    int64_t m_lastDeletedWorkflowId = 0;
    int64_t m_lastCanDeleteWorkflowId = 0;
};

} // namespace tests
} // namespace server
