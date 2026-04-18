#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_field.h"

namespace dto
{

ItemField::ItemField(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemField::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор элемента
    if (m_itemId.has_value())
    {
        result["item_id"] = m_itemId.value();
    }
    // Идентификатор типа поля
    if (m_fieldTypeId.has_value())
    {
        result["field_type_id"] = m_fieldTypeId.value();
    }
    // Значение поля (в сериализованном виде)
    if (m_value.has_value())
    {
        result["value"] = m_value.value();
    }

    return result;
}

bool ItemField::fromJson(const nlohmann::json& json)
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
    // Идентификатор элемента
    if (json.contains("item_id") && !json["item_id"].is_null())
    {
        try
        {
            m_itemId = json["item_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_itemId = std::nullopt;
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
    // Значение поля (в сериализованном виде)
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

bool ItemField::isValid() const
{
    if (!m_itemId.has_value())
    {
        return false;
    }
    if (!m_fieldTypeId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string ItemField::validationError() const
{
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
    }
    if (!m_fieldTypeId.has_value())
    {
        return "Поле «field_type_id» является обязательным для заполнения";
    }


    return "";
}

bool ItemField::operator==(const ItemField& other) const
{
    return
        m_id == other.m_id
        && m_itemId == other.m_itemId
        && m_fieldTypeId == other.m_fieldTypeId
        && m_value == other.m_value
;
}

} // namespace dto