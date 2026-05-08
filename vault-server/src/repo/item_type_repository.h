#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/item_type.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с типами элементов.
 */
class IItemTypeRepository
{
public:
    virtual ~IItemTypeRepository() = default;

    /**
     * @brief Получает список типов элементов с пагинацией и фильтрацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param workflowId Фильтр по рабочему процессу (std::nullopt - все)
     * @param kind Фильтр по виду элемента (std::nullopt - все)
     * @param captionSearch Поиск по названию (пустая строка - без поиска)
     * @return Пара: вектор DTO типов элементов и общее количество.
     */
    virtual std::pair<std::vector<dto::ItemType>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId = std::nullopt,
        std::optional<std::string> kind = std::nullopt,
        const std::string& captionSearch = ""
    ) = 0;

    /**
     * @brief Находит тип элемента по ID.
     * @param id Идентификатор типа элемента
     * @return DTO типа элемента или std::nullopt, если не найден
     */
    virtual std::optional<dto::ItemType> findById(int64_t id) = 0;

    /**
     * @brief Создает новый тип элемента.
     * @param itemType DTO типа элемента
     * @return ID созданного типа или 0 при ошибке
     */
    virtual int64_t create(const dto::ItemType& itemType) = 0;

    /**
     * @brief Обновляет существующий тип элемента.
     * @param itemType DTO с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::ItemType& itemType) = 0;

    /**
     * @brief Удаляет тип элемента.
     * @param id Идентификатор типа элемента
     * @return true если удаление успешно
     */
    virtual bool remove(int64_t id) = 0;

    /**
     * @brief Проверяет существование типа элемента с указанным ID.
     * @param id Идентификатор типа элемента
     * @return true если тип элемента существует
     */
    virtual bool exists(int64_t id) = 0;

    /**
     * @brief Получает все типы элементов для указанного рабочего процесса.
     * @param workflowId Идентификатор рабочего процесса
     * @return Вектор DTO типов элементов
     */
    virtual std::vector<dto::ItemType> findByWorkflow(int64_t workflowId) = 0;

    /**
     * @brief Проверяет, используется ли тип элемента какими-либо элементами или связями.
     * @param id Идентификатор типа элемента
     * @return Количество элементов, использующих этот тип
     */
    virtual int64_t usageCount(int64_t id) = 0;
};

} // namespace repositories
} // namespace server
