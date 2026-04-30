#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/project.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с проектами.
 */
class IProjectRepository
{
public:
    virtual ~IProjectRepository() = default;

    /**
     * @brief Получает список проектов с пагинацией и фильтрацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param parentId Фильтр по родительскому проекту (std::nullopt - все)
     * @param isArchive Фильтр по статусу архивации (std::nullopt - все)
     * @param searchCaption Поиск по названию (пустая строка - без поиска)
     * @return Пара: вектор DTO проектов и общее количество.
     */
    virtual std::pair<std::vector<dto::Project>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> parentId = std::nullopt,
        std::optional<bool> isArchive = std::nullopt,
        const std::string& searchCaption = ""
    ) = 0;

    /**
     * @brief Находит проект по ID.
     * @param id Идентификатор проекта
     * @return DTO проекта или std::nullopt, если не найден
     */
    virtual std::optional<dto::Project> findById(int64_t id) = 0;

    /**
     * @brief Создает новый проект.
     * @param project DTO проекта (поля createdAt/updatedAt будут установлены автоматически)
     * @return ID созданного проекта или 0 при ошибке
     */
    virtual int64_t create(const dto::Project& project) = 0;

    /**
     * @brief Обновляет существующий проект.
     * @param project DTO проекта с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::Project& project) = 0;

    /**
     * @brief Мягкое удаление проекта (архивация).
     * @param id Идентификатор проекта
     * @return true если архивация успешна
     */
    virtual bool archive(int64_t id) = 0;

    /**
     * @brief Восстановление проекта из архива.
     * @param id Идентификатор проекта
     * @return true если восстановление успешно
     */
    virtual bool restore(int64_t id) = 0;

    /**
     * @brief Проверяет существование проекта с указанным ID.
     * @param id Идентификатор проекта
     * @return true если проект существует
     */
    virtual bool exists(int64_t id) = 0;

    /**
     * @brief Получает количество дочерних проектов.
     * @param parentId Идентификатор родительского проекта
     * @return Количество дочерних проектов
     */
    virtual int64_t childCount(int64_t parentId) = 0;
};

} // namespace repositories
} // namespace server
