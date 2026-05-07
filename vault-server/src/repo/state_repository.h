#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/state.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с состояниями.
 */
class IStateRepository
{
public:
    virtual ~IStateRepository() = default;

    /**
     * @brief Получает список состояний с пагинацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param workflowId Фильтр по рабочему процессу (опционально)
     * @return Пара: вектор DTO состояний и общее количество
     */
    virtual std::pair<std::vector<dto::State>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId = std::nullopt
    ) = 0;

    /**
     * @brief Находит состояние по ID.
     * @param id Идентификатор состояния
     * @return DTO состояния или std::nullopt, если не найдено
     */
    virtual std::optional<dto::State> findById(int64_t id) = 0;

    /**
     * @brief Создает новое состояние.
     * @param state DTO состояния
     * @return ID созданного состояния или 0 при ошибке
     */
    virtual int64_t create(const dto::State& state) = 0;

    /**
     * @brief Обновляет состояние.
     * @param state DTO состояния с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::State& state) = 0;

    /**
     * @brief Удаляет состояние по ID (мягкое удаление - архивация).
     * @param id Идентификатор состояния
     * @return true если удаление успешно
     */
    virtual bool remove(int64_t id) = 0;

    /**
     * @brief Получает все состояния рабочего процесса.
     * @param workflowId Идентификатор рабочего процесса
     * @return Вектор DTO состояний, отсортированный по orderNumber
     */
    virtual std::vector<dto::State> findByWorkflowId(int64_t workflowId) = 0;

    /**
     * @brief Проверяет, используется ли состояние элементами.
     * @param stateId Идентификатор состояния
     * @return true если состояние используется
     */
    virtual bool isUsedByItems(int64_t stateId) = 0;
};

} // namespace repositories
} // namespace server
