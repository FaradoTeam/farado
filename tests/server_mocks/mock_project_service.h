#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/project.h"

#include "logic/iproject_service.h"

namespace server
{
namespace tests
{

class MockProjectService : public services::IProjectService
{
public:
    using ProjectsPage = services::ProjectsPage;

    // Настройка результатов
    void setGetProjectsResult(const ProjectsPage& result)
    {
        m_getProjectsResult = result;
    }

    void setGetProjectResult(std::optional<dto::Project> project)
    {
        m_getProjectResult = std::move(project);
    }

    void setCreateProjectResult(std::optional<dto::Project> project)
    {
        m_createProjectResult = std::move(project);
    }

    void setUpdateProjectResult(std::optional<dto::Project> project)
    {
        m_updateProjectResult = std::move(project);
    }

    void setArchiveProjectResult(bool result)
    {
        m_archiveProjectResult = result;
    }

    void setRestoreProjectResult(bool result)
    {
        m_restoreProjectResult = result;
    }

    // Реализация интерфейса
    ProjectsPage projects(
        int page,
        int pageSize,
        std::optional<int64_t> parentId,
        std::optional<bool> isArchive,
        const std::string& searchCaption
    ) override
    {
        m_lastGetProjectsPage = page;
        m_lastGetProjectsPageSize = pageSize;
        m_lastGetProjectsParentId = parentId;
        m_lastGetProjectsIsArchive = isArchive;
        m_lastGetProjectsSearch = searchCaption;
        m_getProjectsCallCount++;
        return m_getProjectsResult;
    }

    std::optional<dto::Project> project(int64_t id) override
    {
        m_lastGetProjectId = id;
        m_getProjectCallCount++;
        return m_getProjectResult;
    }

    std::optional<dto::Project> createProject(const dto::Project& project) override
    {
        m_lastCreatedProject = project;
        m_createProjectCallCount++;
        return m_createProjectResult;
    }

    std::optional<dto::Project> updateProject(const dto::Project& project) override
    {
        m_lastUpdatedProject = project;
        m_updateProjectCallCount++;
        return m_updateProjectResult;
    }

    bool archiveProject(int64_t id) override
    {
        m_lastArchivedProjectId = id;
        m_archiveProjectCallCount++;
        return m_archiveProjectResult;
    }

    bool restoreProject(int64_t id) override
    {
        m_lastRestoredProjectId = id;
        m_restoreProjectCallCount++;
        return m_restoreProjectResult;
    }

    // Методы для проверки вызовов
    int getProjectsCallCount() const { return m_getProjectsCallCount; }
    int getProjectCallCount() const { return m_getProjectCallCount; }
    int createProjectCallCount() const { return m_createProjectCallCount; }
    int updateProjectCallCount() const { return m_updateProjectCallCount; }
    int archiveProjectCallCount() const { return m_archiveProjectCallCount; }
    int restoreProjectCallCount() const { return m_restoreProjectCallCount; }

    int getLastGetProjectsPage() const { return m_lastGetProjectsPage; }
    int getLastGetProjectsPageSize() const { return m_lastGetProjectsPageSize; }
    int64_t getLastGetProjectId() const { return m_lastGetProjectId; }
    const dto::Project& getLastCreatedProject() const { return m_lastCreatedProject; }
    const dto::Project& getLastUpdatedProject() const { return m_lastUpdatedProject; }
    int64_t getLastArchivedProjectId() const { return m_lastArchivedProjectId; }
    int64_t getLastRestoredProjectId() const { return m_lastRestoredProjectId; }

    void reset()
    {
        m_getProjectsCallCount = 0;
        m_getProjectCallCount = 0;
        m_createProjectCallCount = 0;
        m_updateProjectCallCount = 0;
        m_archiveProjectCallCount = 0;
        m_restoreProjectCallCount = 0;

        m_lastGetProjectsPage = 0;
        m_lastGetProjectsPageSize = 0;
        m_lastGetProjectsParentId = std::nullopt;
        m_lastGetProjectsIsArchive = std::nullopt;
        m_lastGetProjectsSearch.clear();
        m_lastGetProjectId = 0;
        m_lastCreatedProject = dto::Project {};
        m_lastUpdatedProject = dto::Project {};
        m_lastArchivedProjectId = 0;
        m_lastRestoredProjectId = 0;
    }

private:
    ProjectsPage m_getProjectsResult;
    std::optional<dto::Project> m_getProjectResult;
    std::optional<dto::Project> m_createProjectResult;
    std::optional<dto::Project> m_updateProjectResult;
    bool m_archiveProjectResult = false;
    bool m_restoreProjectResult = false;

    int m_getProjectsCallCount = 0;
    int m_getProjectCallCount = 0;
    int m_createProjectCallCount = 0;
    int m_updateProjectCallCount = 0;
    int m_archiveProjectCallCount = 0;
    int m_restoreProjectCallCount = 0;

    int m_lastGetProjectsPage = 0;
    int m_lastGetProjectsPageSize = 0;
    std::optional<int64_t> m_lastGetProjectsParentId;
    std::optional<bool> m_lastGetProjectsIsArchive;
    std::string m_lastGetProjectsSearch;
    int64_t m_lastGetProjectId = 0;
    dto::Project m_lastCreatedProject;
    dto::Project m_lastUpdatedProject;
    int64_t m_lastArchivedProjectId = 0;
    int64_t m_lastRestoredProjectId = 0;
};

} // namespace tests
} // namespace server
