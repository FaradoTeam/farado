#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/edge.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с переходами.
 */
class IEdgeRepository
{
public:
    virtual ~IEdgeRepository() = default;

    /**
     * @brief Получает список переходов с пагинацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param beginStateId Фильтр по начальному состоянию (опционально)
     * @param endStateId Фильтр по конечному состоянию (опционально)
     * @return Пара: вектор DTO переходов и общее количество
     */
    virtual std::pair<std::vector<dto::Edge>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> beginStateId = std::nullopt,
        std::optional<int64_t> endStateId = std::nullopt
    ) = 0;

    /**
     * @brief Находит переход по ID.
     * @param id Идентификатор перехода
     * @return DTO перехода или std::nullopt, если не найден
     */
    virtual std::optional<dto::Edge> findById(int64_t id) = 0;

    /**
     * @brief Создает новый переход.
     * @param edge DTO перехода
     * @return ID созданного перехода или 0 при ошибке
     */
    virtual int64_t create(const dto::Edge& edge) = 0;

    /**
     * @brief Удаляет переход по ID.
     * @param id Идентификатор перехода
     * @return true если удаление успешно
     */
    virtual bool remove(int64_t id) = 0;

    /**
     * @brief Получает все переходы, связанные с состоянием.
     * @param stateId Идентификатор состояния
     * @return Вектор DTO переходов, где состояние является начальным или конечным
     */
    virtual std::vector<dto::Edge> findByStateId(int64_t stateId) = 0;

    /**
     * @brief Проверяет существование перехода между состояниями.
     * @param beginStateId Начальное состояние
     * @param endStateId Конечное состояние
     * @return true если переход существует
     */
    virtual bool exists(int64_t beginStateId, int64_t endStateId) = 0;

    /**
     * @brief Получает все переходы для рабочего процесса.
     * @param workflowId Идентификатор рабочего процесса
     * @return Вектор DTO переходов
     */
    virtual std::vector<dto::Edge> findByWorkflowId(int64_t workflowId) = 0;
};

} // namespace repositories
} // namespace server
