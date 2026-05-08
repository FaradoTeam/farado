#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/field_type.h"

namespace server
{
namespace services
{

/**
 * @brief Структура для возврата результатов пагинированного списка типов полей.
 */
struct FieldTypesPage
{
    std::vector<dto::FieldType> fieldTypes;
    int64_t totalCount = 0;
};

/**
 * @brief Интерфейс сервиса для управления типами полей.
 */
class IFieldTypeService
{
public:
    virtual ~IFieldTypeService() = default;

    /**
     * @brief Получает список типов полей с пагинацией и фильтрацией.
     */
    virtual FieldTypesPage fieldTypes(
        int page,
        int pageSize,
        std::optional<int64_t> itemTypeId = std::nullopt,
        std::optional<std::string> valueType = std::nullopt,
        const std::string& searchCaption = ""
    ) = 0;

    /**
     * @brief Получает тип поля по ID.
     */
    virtual std::optional<dto::FieldType> fieldType(int64_t id) = 0;

    /**
     * @brief Создает новый тип поля.
     */
    virtual std::optional<dto::FieldType> createFieldType(const dto::FieldType& fieldType) = 0;

    /**
     * @brief Обновляет существующий тип поля.
     */
    virtual std::optional<dto::FieldType> updateFieldType(const dto::FieldType& fieldType) = 0;

    /**
     * @brief Удаляет тип поля.
     */
    virtual bool deleteFieldType(int64_t id) = 0;

    /**
     * @brief Получает типы полей для типа элемента.
     */
    virtual std::vector<dto::FieldType> fieldTypesByItemType(int64_t itemTypeId) = 0;
};

} // namespace services
} // namespace server
