#pragma once

#include <memory>

#include "logic/iproject_service.h"
#include "repo/project_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса для управления проектами.
 */
class ProjectService final : public IProjectService
{
public:
    explicit ProjectService(std::shared_ptr<repositories::IProjectRepository> projectRepo);

    ProjectsPage projects(
        int page,
        int pageSize,
        std::optional<int64_t> parentId,
        std::optional<bool> isArchive,
        const std::string& searchCaption
    ) override;
    std::optional<dto::Project> project(int64_t id) override;
    std::optional<dto::Project> createProject(const dto::Project& project) override;
    std::optional<dto::Project> updateProject(const dto::Project& project) override;
    bool archiveProject(int64_t id) override;
    bool restoreProject(int64_t id) override;

private:
    std::shared_ptr<repositories::IProjectRepository> m_projectRepo;
};

} // namespace services
} // namespace server
