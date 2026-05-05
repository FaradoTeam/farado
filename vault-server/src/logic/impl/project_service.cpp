#include "common/log/log.h"

#include "project_service.h"

namespace server
{
namespace services
{

ProjectService::ProjectService(std::shared_ptr<repositories::IProjectRepository> projectRepo)
    : m_projectRepo(std::move(projectRepo))
{
    if (!m_projectRepo)
    {
        throw std::runtime_error("ProjectRepository cannot be null");
    }
}

ProjectsPage ProjectService::projects(
    int page,
    int pageSize,
    std::optional<int64_t> parentId,
    std::optional<bool> isArchive,
    const std::string& searchCaption
)
{
    if (page < 1)
        page = 1;
    if (pageSize < 1)
        pageSize = 20;

    // TODO: Здесь нужно добавить проверку прав доступа пользователя,
    // чтобы возвращать только те проекты, которые ему доступны.

    auto [projectList, total] = m_projectRepo->findAll(
        page,
        pageSize,
        parentId,
        isArchive,
        searchCaption
    );
    return { projectList, total };
}

std::optional<dto::Project> ProjectService::project(int64_t id)
{
    // TODO: Здесь будет проверка прав пользователя на чтение конкретного проекта.
    return m_projectRepo->findById(id);
}

std::optional<dto::Project> ProjectService::createProject(const dto::Project& project)
{
    // Валидация на уровне бизнес-логики
    if (!project.caption.has_value() || project.caption->empty())
    {
        LOG_WARN << "createProject: название проекта не может быть пустым";
        return std::nullopt;
    }

    // TODO: Проверить права пользователя на создание корневого/дочернего проекта.

    const int64_t newId = m_projectRepo->create(project);
    if (newId <= 0)
    {
        LOG_ERROR << "createProject: ошибка при создании проекта в репозитории";
        return std::nullopt;
    }

    LOG_INFO << "Создан новый проект с id=" << newId;
    return m_projectRepo->findById(newId); // Возвращаем свежесозданный проект
}

std::optional<dto::Project> ProjectService::updateProject(const dto::Project& project)
{
    if (!project.id.has_value())
    {
        LOG_WARN << "updateProject: отсутствует ID проекта";
        return std::nullopt;
    }

    // TODO: Проверить права пользователя на редактирование проекта.

    // Проверяем, существует ли проект
    if (!m_projectRepo->exists(project.id.value()))
    {
        LOG_WARN << "updateProject: проект с id=" << project.id.value() << " не найден";
        return std::nullopt;
    }

    if (!m_projectRepo->update(project))
    {
        LOG_ERROR << "updateProject: ошибка при обновлении проекта в репозитории";
        return std::nullopt;
    }

    LOG_INFO << "Проект с id=" << project.id.value() << " успешно обновлен";
    return m_projectRepo->findById(project.id.value());
}

bool ProjectService::archiveProject(int64_t id)
{
    // TODO: Проверить права пользователя на архивацию проекта.
    if (!m_projectRepo->exists(id))
    {
        LOG_WARN << "archiveProject: проект с id=" << id << " не найден";
        return false;
    }

    if (m_projectRepo->archive(id))
    {
        LOG_INFO << "Проект с id=" << id << " успешно архивирован";
        return true;
    }

    LOG_ERROR << "archiveProject: ошибка при архивации проекта с id=" << id;
    return false;
}

bool ProjectService::restoreProject(int64_t id)
{
    // TODO: Проверить права пользователя на восстановление проекта.
    if (!m_projectRepo->exists(id))
    {
        LOG_WARN << "restoreProject: проект с id=" << id << " не найден";
        return false;
    }

    if (m_projectRepo->restore(id))
    {
        LOG_INFO << "Проект с id=" << id << " успешно восстановлен";
        return true;
    }

    LOG_ERROR << "restoreProject: ошибка при восстановлении проекта с id=" << id;
    return false;
}

} // namespace services
} // namespace server
