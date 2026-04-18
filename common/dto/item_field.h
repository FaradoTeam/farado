#pragma once

#include <optional>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "common/helpers/print_helpers.h"


namespace dto
{

/**
 * @brief ItemField DTO
 */
class ItemField final
{
public:
    using Ptr = std::shared_ptr<ItemField>;
    using ConstPtr = std::shared_ptr<const ItemField>;

public:
    ItemField() = default;
    explicit ItemField(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор
    const std::optional<int64_t>& id() const { return m_id; }
    bool hasId() const { return m_id.has_value(); }
    void setId(const int64_t& value)
    {
        m_id = value;
    }
    void setId(int64_t&& value)
    {
        m_id = std::move(value);
    }
    void clearId()
    {
        m_id = std::nullopt;
    }

    // Поле: «itemId» Идентификатор элемента
    const std::optional<int64_t>& itemId() const { return m_itemId; }
    bool hasItemId() const { return m_itemId.has_value(); }
    void setItemId(const int64_t& value)
    {
        m_itemId = value;
    }
    void setItemId(int64_t&& value)
    {
        m_itemId = std::move(value);
    }
    void clearItemId()
    {
        m_itemId = std::nullopt;
    }

    // Поле: «fieldTypeId» Идентификатор типа поля
    const std::optional<int64_t>& fieldTypeId() const { return m_fieldTypeId; }
    bool hasFieldTypeId() const { return m_fieldTypeId.has_value(); }
    void setFieldTypeId(const int64_t& value)
    {
        m_fieldTypeId = value;
    }
    void setFieldTypeId(int64_t&& value)
    {
        m_fieldTypeId = std::move(value);
    }
    void clearFieldTypeId()
    {
        m_fieldTypeId = std::nullopt;
    }

    // Поле: «value» Значение поля (в сериализованном виде)
    const std::optional<std::string>& value() const { return m_value; }
    bool hasValue() const { return m_value.has_value(); }
    void setValue(const std::string& value)
    {
        m_value = value;
    }
    void setValue(std::string&& value)
    {
        m_value = std::move(value);
    }
    void clearValue()
    {
        m_value = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ItemField& other) const;
    bool operator!=(const ItemField& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ItemField& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemId;
    std::optional<int64_t> m_fieldTypeId;
    std::optional<std::string> m_value;
};

inline std::ostream& operator<<(std::ostream& os, const ItemField& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto