#pragma once

#include <optional>
#include <vector>

#include "common/dto/phase.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с фазами.
 */
class IPhaseRepository
{
public:
    virtual ~IPhaseRepository() = default;

    /**
     * @brief Получает список фаз с пагинацией и фильтрацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param projectId Фильтр по проекту (std::nullopt - все)
     * @param isArchive Фильтр по статусу архивации (std::nullopt - все)
     * @return Пара: вектор DTO фаз и общее количество.
     */
    virtual std::pair<std::vector<dto::Phase>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> projectId = std::nullopt,
        std::optional<bool> isArchive = std::nullopt
    ) = 0;

    /**
     * @brief Находит фазу по ID.
     * @param id Идентификатор фазы
     * @return DTO фазы или std::nullopt, если не найдена
     */
    virtual std::optional<dto::Phase> findById(int64_t id) = 0;

    /**
     * @brief Создает новую фазу.
     * @param phase DTO фазы
     * @return ID созданной фазы или 0 при ошибке
     */
    virtual int64_t create(const dto::Phase& phase) = 0;

    /**
     * @brief Обновляет существующую фазу.
     * @param phase DTO фазы с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::Phase& phase) = 0;

    /**
     * @brief Мягкое удаление фазы (архивация).
     * @param id Идентификатор фазы
     * @return true если архивация успешна
     */
    virtual bool archive(int64_t id) = 0;

    /**
     * @brief Восстановление фазы из архива.
     * @param id Идентификатор фазы
     * @return true если восстановление успешно
     */
    virtual bool restore(int64_t id) = 0;

    /**
     * @brief Проверяет существование фазы с указанным ID.
     * @param id Идентификатор фазы
     * @return true если фаза существует
     */
    virtual bool exists(int64_t id) = 0;

    /**
     * @brief Получает все фазы проекта (без пагинации).
     * @param projectId Идентификатор проекта
     * @param includeArchived Включать ли архивные фазы
     * @return Вектор DTO фаз
     */
    virtual std::vector<dto::Phase> findByProject(
        int64_t projectId,
        bool includeArchived = false
    ) = 0;
};

} // namespace repositories
} // namespace server
