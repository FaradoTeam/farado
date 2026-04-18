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
 * @brief FieldTypePossibleValue DTO
 */
class FieldTypePossibleValue final
{
public:
    using Ptr = std::shared_ptr<FieldTypePossibleValue>;
    using ConstPtr = std::shared_ptr<const FieldTypePossibleValue>;

public:
    FieldTypePossibleValue() = default;
    explicit FieldTypePossibleValue(const nlohmann::json& json);

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

    // Поле: «value» Значение для выбора
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
    bool operator==(const FieldTypePossibleValue& other) const;
    bool operator!=(const FieldTypePossibleValue& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const FieldTypePossibleValue& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_fieldTypeId;
    std::optional<std::string> m_value;
};

inline std::ostream& operator<<(std::ostream& os, const FieldTypePossibleValue& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto