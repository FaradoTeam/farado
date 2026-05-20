#pragma once

#include <memory>

#include "logic/iworkflow_service.h"

#include "repo/edge_repository.h"
#include "repo/state_repository.h"
#include "repo/workflow_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса рабочих процессов.
 */
class WorkflowService final : public IWorkflowService
{
public:
    explicit WorkflowService(
        std::shared_ptr<repositories::IWorkflowRepository> workflowRepo,
        std::shared_ptr<repositories::IStateRepository> stateRepo,
        std::shared_ptr<repositories::IEdgeRepository> edgeRepo
    );

    WorkflowsPage workflows(int page, int pageSize) override;
    std::optional<dto::Workflow> workflow(int64_t id) override;
    std::optional<dto::Workflow> createWorkflow(const dto::Workflow& workflow) override;
    std::optional<dto::Workflow> updateWorkflow(const dto::Workflow& workflow) override;
    WorkflowResult deleteWorkflow(int64_t id) override;
    bool canDeleteWorkflow(int64_t id) override;

private:
    /**
     * @brief Валидирует данные рабочего процесса перед созданием.
     */
    bool validateWorkflow(const dto::Workflow& workflow, std::string& errorMessage);

private:
    std::shared_ptr<repositories::IWorkflowRepository> m_workflowRepo;
    std::shared_ptr<repositories::IStateRepository> m_stateRepo;
    std::shared_ptr<repositories::IEdgeRepository> m_edgeRepo;
};

} // namespace services
} // namespace server
