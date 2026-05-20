#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/edge.h"

#include "logic/iedge_service.h"

namespace server
{
namespace tests
{

class MockEdgeService : public services::IEdgeService
{
public:
    using EdgesPage = services::EdgesPage;
    using EdgeResult = services::EdgeResult;

    // Настройка результатов
    void setEdgesResult(const EdgesPage& result)
    {
        m_edgesResult = result;
    }

    void setEdgeResult(std::optional<dto::Edge> edge)
    {
        m_edgeResult = std::move(edge);
    }

    void setCreateEdgeResult(std::optional<dto::Edge> edge)
    {
        m_createEdgeResult = std::move(edge);
    }

    void setDeleteEdgeResult(const EdgeResult& result)
    {
        m_deleteEdgeResult = result;
    }

    void setWorkflowEdgesResult(const std::vector<dto::Edge>& edges)
    {
        m_workflowEdgesResult = edges;
    }

    void setValidateEdgeResult(const EdgeResult& result)
    {
        m_validateEdgeResult = result;
    }

    // Реализация интерфейса
    EdgesPage edges(
        int page, int pageSize,
        std::optional<int64_t> beginStateId = std::nullopt,
        std::optional<int64_t> endStateId = std::nullopt
    ) override
    {
        m_lastEdgesPage = page;
        m_lastEdgesPageSize = pageSize;
        m_lastEdgesBeginStateId = beginStateId;
        m_lastEdgesEndStateId = endStateId;
        m_edgesCallCount++;
        return m_edgesResult;
    }

    std::optional<dto::Edge> edge(int64_t id) override
    {
        m_lastEdgeId = id;
        m_edgeCallCount++;
        return m_edgeResult;
    }

    std::optional<dto::Edge> createEdge(const dto::Edge& edge) override
    {
        m_lastCreatedEdge = edge;
        m_createEdgeCallCount++;
        return m_createEdgeResult;
    }

    EdgeResult deleteEdge(int64_t id) override
    {
        m_lastDeletedEdgeId = id;
        m_deleteEdgeCallCount++;
        return m_deleteEdgeResult;
    }

    std::vector<dto::Edge> getWorkflowEdges(int64_t workflowId) override
    {
        m_lastWorkflowEdgesId = workflowId;
        m_workflowEdgesCallCount++;
        return m_workflowEdgesResult;
    }

    EdgeResult validateEdge(int64_t beginStateId, int64_t endStateId) override
    {
        m_lastValidateBeginStateId = beginStateId;
        m_lastValidateEndStateId = endStateId;
        m_validateEdgeCallCount++;
        return m_validateEdgeResult;
    }

    // Методы для проверки вызовов
    int getEdgesCallCount() const { return m_edgesCallCount; }
    int getEdgeCallCount() const { return m_edgeCallCount; }
    int getCreateEdgeCallCount() const { return m_createEdgeCallCount; }
    int getDeleteEdgeCallCount() const { return m_deleteEdgeCallCount; }
    int getWorkflowEdgesCallCount() const { return m_workflowEdgesCallCount; }

    int getLastEdgesPage() const { return m_lastEdgesPage; }
    int getLastEdgesPageSize() const { return m_lastEdgesPageSize; }
    std::optional<int64_t> getLastEdgesBeginStateId() const { return m_lastEdgesBeginStateId; }
    std::optional<int64_t> getLastEdgesEndStateId() const { return m_lastEdgesEndStateId; }
    int64_t getLastEdgeId() const { return m_lastEdgeId; }
    const dto::Edge& getLastCreatedEdge() const { return m_lastCreatedEdge; }
    int64_t getLastDeletedEdgeId() const { return m_lastDeletedEdgeId; }
    int64_t getLastWorkflowEdgesId() const { return m_lastWorkflowEdgesId; }

    void reset()
    {
        m_edgesCallCount = 0;
        m_edgeCallCount = 0;
        m_createEdgeCallCount = 0;
        m_deleteEdgeCallCount = 0;
        m_workflowEdgesCallCount = 0;
        m_validateEdgeCallCount = 0;
        m_lastEdgesPage = 0;
        m_lastEdgesPageSize = 0;
        m_lastEdgesBeginStateId = std::nullopt;
        m_lastEdgesEndStateId = std::nullopt;
        m_lastEdgeId = 0;
        m_lastCreatedEdge = dto::Edge {};
        m_lastDeletedEdgeId = 0;
        m_lastWorkflowEdgesId = 0;
        m_lastValidateBeginStateId = 0;
        m_lastValidateEndStateId = 0;
    }

private:
    EdgesPage m_edgesResult;
    std::optional<dto::Edge> m_edgeResult;
    std::optional<dto::Edge> m_createEdgeResult;
    EdgeResult m_deleteEdgeResult;
    std::vector<dto::Edge> m_workflowEdgesResult;
    EdgeResult m_validateEdgeResult;

    int m_edgesCallCount = 0;
    int m_edgeCallCount = 0;
    int m_createEdgeCallCount = 0;
    int m_deleteEdgeCallCount = 0;
    int m_workflowEdgesCallCount = 0;
    int m_validateEdgeCallCount = 0;

    int m_lastEdgesPage = 0;
    int m_lastEdgesPageSize = 0;
    std::optional<int64_t> m_lastEdgesBeginStateId;
    std::optional<int64_t> m_lastEdgesEndStateId;
    int64_t m_lastEdgeId = 0;
    dto::Edge m_lastCreatedEdge;
    int64_t m_lastDeletedEdgeId = 0;
    int64_t m_lastWorkflowEdgesId = 0;
    int64_t m_lastValidateBeginStateId = 0;
    int64_t m_lastValidateEndStateId = 0;
};

} // namespace tests
} // namespace server
