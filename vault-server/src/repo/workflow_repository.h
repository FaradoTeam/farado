#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/workflow.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с рабочими процессами.
 */
class IWorkflowRepository
{
public:
    virtual ~IWorkflowRepository() = default;

    /**
     * @brief Получает список рабочих процессов с пагинацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @return Пара: вектор DTO рабочих процессов и общее количество
     */
    virtual std::pair<std::vector<dto::Workflow>, int64_t> findAll(
        int page,
        int pageSize
    ) = 0;

    /**
     * @brief Находит рабочий процесс по ID.
     * @param id Идентификатор рабочего процесса
     * @return DTO рабочего процесса или std::nullopt, если не найден
     */
    virtual std::optional<dto::Workflow> findById(int64_t id) = 0;

    /**
     * @brief Создает новый рабочий процесс.
     * @param workflow DTO рабочего процесса
     * @return ID созданного рабочего процесса или 0 при ошибке
     */
    virtual int64_t create(const dto::Workflow& workflow) = 0;

    /**
     * @brief Обновляет рабочий процесс.
     * @param workflow DTO рабочего процесса с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::Workflow& workflow) = 0;

    /**
     * @brief Удаляет рабочий процесс по ID.
     * @param id Идентификатор рабочего процесса
     * @return true если удаление успешно
     */
    virtual bool remove(int64_t id) = 0;

    /**
     * @brief Проверяет существование рабочего процесса с указанным названием.
     * @param caption Название для проверки
     * @return true если рабочий процесс существует
     */
    virtual bool existsByCaption(const std::string& caption) = 0;
};

} // namespace repositories
} // namespace server
