#pragma once

#include <memory>

#include "logic/iedge_service.h"
#include "repo/edge_repository.h"
#include "repo/state_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса переходов.
 */
class EdgeService final : public IEdgeService
{
public:
    explicit EdgeService(
        std::shared_ptr<repositories::IEdgeRepository> edgeRepo,
        std::shared_ptr<repositories::IStateRepository> stateRepo
    );

    EdgesPage edges(
        int page,
        int pageSize,
        std::optional<int64_t> beginStateId = std::nullopt,
        std::optional<int64_t> endStateId = std::nullopt
    ) override;
    std::optional<dto::Edge> edge(int64_t id) override;
    std::optional<dto::Edge> createEdge(const dto::Edge& edge) override;
    EdgeResult deleteEdge(int64_t id) override;
    std::vector<dto::Edge> getWorkflowEdges(int64_t workflowId) override;
    EdgeResult validateEdge(int64_t beginStateId, int64_t endStateId) override;

private:
    std::shared_ptr<repositories::IEdgeRepository> m_edgeRepo;
    std::shared_ptr<repositories::IStateRepository> m_stateRepo;
};

} // namespace services
} // namespace server
