#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "field_type.h"

namespace dto
{

FieldType::FieldType(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json FieldType::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор типа элемента
    if (m_itemTypeId.has_value())
    {
        result["item_type_id"] = m_itemTypeId.value();
    }
    // Название поля
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание поля
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }
    // Тип значения
    if (m_valueType.has_value())
    {
        result["value_type"] = m_valueType.value();
    }
    // Отображать поле на канбан-доске
    if (m_isBoardVisible.has_value())
    {
        result["is_board_visible"] = m_isBoardVisible.value();
    }

    return result;
}

bool FieldType::fromJson(const nlohmann::json& json)
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
    // Идентификатор типа элемента
    if (json.contains("item_type_id") && !json["item_type_id"].is_null())
    {
        try
        {
            m_itemTypeId = json["item_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_itemTypeId = std::nullopt;
    }
    // Название поля
    if (json.contains("caption") && !json["caption"].is_null())
    {
        try
        {
            m_caption = json["caption"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_caption = std::nullopt;
    }
    // Описание поля
    if (json.contains("description") && !json["description"].is_null())
    {
        try
        {
            m_description = json["description"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_description = std::nullopt;
    }
    // Тип значения
    if (json.contains("value_type") && !json["value_type"].is_null())
    {
        try
        {
            m_valueType = json["value_type"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_valueType = std::nullopt;
    }
    // Отображать поле на канбан-доске
    if (json.contains("is_board_visible") && !json["is_board_visible"].is_null())
    {
        try
        {
            m_isBoardVisible = json["is_board_visible"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isBoardVisible = std::nullopt;
    }

    return success;
}

bool FieldType::isValid() const
{
    if (!m_itemTypeId.has_value())
    {
        return false;
    }
    if (!m_caption.has_value())
    {
        return false;
    }
    if (!m_valueType.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_caption.value().empty())
    {
        return false;
    }
    if (m_valueType.value().empty())
    {
        return false;
    }

    return true;
}

std::string FieldType::validationError() const
{
    if (!m_itemTypeId.has_value())
    {
        return "Поле «item_type_id» является обязательным для заполнения";
    }
    if (!m_caption.has_value())
    {
        return "Поле «caption» является обязательным для заполнения";
    }
    if (!m_valueType.has_value())
    {
        return "Поле «value_type» является обязательным для заполнения";
    }

    if (m_caption.value().empty())
    {
        return "Поле «caption» не может быть пустой строкой";
    }
    if (m_valueType.value().empty())
    {
        return "Поле «value_type» не может быть пустой строкой";
    }

    return "";
}

bool FieldType::operator==(const FieldType& other) const
{
    return
        m_id == other.m_id
        && m_itemTypeId == other.m_itemTypeId
        && m_caption == other.m_caption
        && m_description == other.m_description
        && m_valueType == other.m_valueType
        && m_isBoardVisible == other.m_isBoardVisible
;
}

} // namespace dto