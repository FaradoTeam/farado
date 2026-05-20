#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/item_type.h"

namespace server
{
namespace services
{

/**
 * @brief Структура для возврата результатов пагинированного списка типов элементов.
 */
struct ItemTypesPage
{
    std::vector<dto::ItemType> itemTypes;
    int64_t totalCount = 0;
};

/**
 * @brief Интерфейс сервиса для управления типами элементов.
 */
class IItemTypeService
{
public:
    virtual ~IItemTypeService() = default;

    /**
     * @brief Получает список типов элементов с пагинацией и фильтрацией.
     */
    virtual ItemTypesPage itemTypes(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId = std::nullopt,
        std::optional<std::string> kind = std::nullopt,
        const std::string& searchCaption = ""
    ) = 0;

    /**
     * @brief Получает тип элемента по ID.
     */
    virtual std::optional<dto::ItemType> itemType(int64_t id) = 0;

    /**
     * @brief Создает новый тип элемента.
     */
    virtual std::optional<dto::ItemType> createItemType(const dto::ItemType& itemType) = 0;

    /**
     * @brief Обновляет существующий тип элемента.
     */
    virtual std::optional<dto::ItemType> updateItemType(const dto::ItemType& itemType) = 0;

    /**
     * @brief Удаляет тип элемента.
     */
    virtual bool deleteItemType(int64_t id) = 0;

    /**
     * @brief Получает типы элементов для рабочего процесса.
     */
    virtual std::vector<dto::ItemType> itemTypesByWorkflow(int64_t workflowId) = 0;
};

} // namespace services
} // namespace server
