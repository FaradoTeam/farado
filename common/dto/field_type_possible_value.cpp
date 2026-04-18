#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "field_type_possible_value.h"

namespace dto
{

FieldTypePossibleValue::FieldTypePossibleValue(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json FieldTypePossibleValue::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор типа поля
    if (m_fieldTypeId.has_value())
    {
        result["field_type_id"] = m_fieldTypeId.value();
    }
    // Значение для выбора
    if (m_value.has_value())
    {
        result["value"] = m_value.value();
    }

    return result;
}

bool FieldTypePossibleValue::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор
    if (json.contains("id") && !json["id"].is_null())
    {
        try
        {
            m_id = json["id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_id = std::nullopt;
    }
    // Идентификатор типа поля
    if (json.contains("field_type_id") && !json["field_type_id"].is_null())
    {
        try
        {
            m_fieldTypeId = json["field_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_fieldTypeId = std::nullopt;
    }
    // Значение для выбора
    if (json.contains("value") && !json["value"].is_null())
    {
        try
        {
            m_value = json["value"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_value = std::nullopt;
    }

    return success;
}

bool FieldTypePossibleValue::isValid() const
{
    if (!m_fieldTypeId.has_value())
    {
        return false;
    }
    if (!m_value.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_value.value().empty())
    {
        return false;
    }

    return true;
}

std::string FieldTypePossibleValue::validationError() const
{
    if (!m_fieldTypeId.has_value())
    {
        return "Поле «field_type_id» является обязательным для заполнения";
    }
    if (!m_value.has_value())
    {
        return "Поле «value» является обязательным для заполнения";
    }

    if (m_value.value().empty())
    {
        return "Поле «value» не может быть пустой строкой";
    }

    return "";
}

bool FieldTypePossibleValue::operator==(const FieldTypePossibleValue& other) const
{
    return
        m_id == other.m_id
        && m_fieldTypeId == other.m_fieldTypeId
        && m_value == other.m_value
;
}

} // namespace dto