#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/dto/field_type.h"

namespace server
{
namespace repositories
{

/**
 * @brief Абстрактный интерфейс репозитория для работы с типами полей.
 */
class IFieldTypeRepository
{
public:
    virtual ~IFieldTypeRepository() = default;

    /**
     * @brief Получает список типов полей с пагинацией и фильтрацией.
     * @param page Номер страницы (начиная с 1)
     * @param pageSize Количество записей на странице
     * @param itemTypeId Фильтр по типу элемента (std::nullopt - все)
     * @param valueType Фильтр по типу значения (std::nullopt - все)
     * @param captionSearch Поиск по названию (пустая строка - без поиска)
     * @return Пара: вектор DTO типов полей и общее количество.
     */
    virtual std::pair<std::vector<dto::FieldType>, int64_t> findAll(
        int page,
        int pageSize,
        std::optional<int64_t> itemTypeId = std::nullopt,
        std::optional<std::string> valueType = std::nullopt,
        const std::string& captionSearch = ""
    ) = 0;

    /**
     * @brief Находит тип поля по ID.
     * @param id Идентификатор типа поля
     * @return DTO типа поля или std::nullopt, если не найден
     */
    virtual std::optional<dto::FieldType> findById(int64_t id) = 0;

    /**
     * @brief Создает новый тип поля.
     * @param fieldType DTO типа поля
     * @return ID созданного типа поля или 0 при ошибке
     */
    virtual int64_t create(const dto::FieldType& fieldType) = 0;

    /**
     * @brief Обновляет существующий тип поля.
     * @param fieldType DTO с новыми данными. Поле id обязательно.
     * @return true если обновление успешно
     */
    virtual bool update(const dto::FieldType& fieldType) = 0;

    /**
     * @brief Удаляет тип поля.
     * @param id Идентификатор типа поля
     * @return true если удаление успешно
     */
    virtual bool remove(int64_t id) = 0;

    /**
     * @brief Проверяет существование типа поля с указанным ID.
     * @param id Идентификатор типа поля
     * @return true если тип поля существует
     */
    virtual bool exists(int64_t id) = 0;

    /**
     * @brief Получает все типы полей для указанного типа элемента.
     * @param itemTypeId Идентификатор типа элемента
     * @return Вектор DTO типов полей
     */
    virtual std::vector<dto::FieldType> findByItemType(int64_t itemTypeId) = 0;

    /**
     * @brief Проверяет, используется ли тип поля какими-либо значениями полей элементов.
     * @param id Идентификатор типа поля
     * @return Количество записей, использующих этот тип поля
     */
    virtual int64_t usageCount(int64_t id) = 0;
};

} // namespace repositories
} // namespace server
