// tests/server_mocks/mock_project_repository.h
#pragma once

#include <functional>
#include <optional>
#include <string>

#include "common/dto/project.h"

#include "repo/project_repository.h"

namespace server
{
namespace tests
{

class MockProjectRepository : public repositories::IProjectRepository
{
public:
    void setFindAllResult(std::pair<std::vector<dto::Project>, int64_t> result)
    {
        m_findAllResult = std::move(result);
        m_findAllCallback = [this](
                                int, int, std::optional<int64_t>,
                                std::optional<bool>, const std::string&
                            )
        {
            return m_findAllResult;
        };
    }

    void setFindAllCallback(
        std::function<std::pair<std::vector<dto::Project>, int64_t>(
            int, int, std::optional<int64_t>, std::optional<bool>, const std::string&
        )>
            callback
    )
    {
        m_findAllCallback = std::move(callback);
    }

    void setFindByIdResult(std::optional<dto::Project> project)
    {
        m_findByIdResult = std::move(project);
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

    void setChildCountResult(int64_t count)
    {
        m_childCountResult = count;
    }

    // Реализация интерфейса
    std::pair<std::vector<dto::Project>, int64_t> findAll(
        int page, int pageSize,
        std::optional<int64_t> parentId,
        std::optional<bool> isArchive,
        const std::string& searchCaption
    ) override
    {
        m_lastFindAllPage = page;
        m_lastFindAllPageSize = pageSize;
        m_lastFindAllParentId = parentId;
        m_lastFindAllIsArchive = isArchive;
        m_lastFindAllSearch = searchCaption;
        m_findAllCallCount++;

        if (m_findAllCallback)
        {
            return m_findAllCallback(page, pageSize, parentId, isArchive, searchCaption);
        }
        return m_findAllResult;
    }

    std::optional<dto::Project> findById(int64_t id) override
    {
        m_lastFindById = id;
        m_findByIdCallCount++;
        return m_findByIdResult;
    }

    int64_t create(const dto::Project& project) override
    {
        m_lastCreatedProject = project;
        m_createCallCount++;
        return m_createResult;
    }

    bool update(const dto::Project& project) override
    {
        m_lastUpdatedProject = project;
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

    int64_t childCount(int64_t parentId) override
    {
        m_lastChildCountParentId = parentId;
        m_childCountCallCount++;
        return m_childCountResult;
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
    std::optional<int64_t> getLastFindAllParentId() const { return m_lastFindAllParentId; }
    int64_t getLastFindById() const { return m_lastFindById; }
    const dto::Project& getLastCreatedProject() const { return m_lastCreatedProject; }
    const dto::Project& getLastUpdatedProject() const { return m_lastUpdatedProject; }
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
        m_childCountCallCount = 0;
        m_findAllCallback = nullptr;
    }

private:
    std::pair<std::vector<dto::Project>, int64_t> m_findAllResult;
    std::optional<dto::Project> m_findByIdResult;
    int64_t m_createResult = 0;
    bool m_updateResult = false;
    bool m_archiveResult = false;
    bool m_restoreResult = false;
    bool m_existsResult = false;
    int64_t m_childCountResult = 0;

    std::function<std::pair<std::vector<dto::Project>, int64_t>(
        int, int, std::optional<int64_t>, std::optional<bool>, const std::string&
    )>
        m_findAllCallback;

    int m_findAllCallCount = 0;
    int m_findByIdCallCount = 0;
    int m_createCallCount = 0;
    int m_updateCallCount = 0;
    int m_archiveCallCount = 0;
    int m_restoreCallCount = 0;
    int m_existsCallCount = 0;
    int m_childCountCallCount = 0;

    int m_lastFindAllPage = 0;
    int m_lastFindAllPageSize = 0;
    std::optional<int64_t> m_lastFindAllParentId;
    std::optional<bool> m_lastFindAllIsArchive;
    std::string m_lastFindAllSearch;
    int64_t m_lastFindById = 0;
    dto::Project m_lastCreatedProject;
    dto::Project m_lastUpdatedProject;
    int64_t m_lastArchivedId = 0;
    int64_t m_lastRestoredId = 0;
    int64_t m_lastExistsId = 0;
    int64_t m_lastChildCountParentId = 0;
};

} // namespace tests
} // namespace server
