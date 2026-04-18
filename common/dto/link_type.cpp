#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "link_type.h"

namespace dto
{

LinkType::LinkType(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json LinkType::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Исходный тип элемента
    if (m_sourceItemTypeId.has_value())
    {
        result["source_item_type_id"] = m_sourceItemTypeId.value();
    }
    // Целевой тип элемента
    if (m_destinationItemTypeId.has_value())
    {
        result["destination_item_type_id"] = m_destinationItemTypeId.value();
    }
    // Флаг двунаправленной связи
    if (m_isBidirectional.has_value())
    {
        result["is_bidirectional"] = m_isBidirectional.value();
    }
    // Название типа связи
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }

    return result;
}

bool LinkType::fromJson(const nlohmann::json& json)
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
    // Исходный тип элемента
    if (json.contains("source_item_type_id") && !json["source_item_type_id"].is_null())
    {
        try
        {
            m_sourceItemTypeId = json["source_item_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_sourceItemTypeId = std::nullopt;
    }
    // Целевой тип элемента
    if (json.contains("destination_item_type_id") && !json["destination_item_type_id"].is_null())
    {
        try
        {
            m_destinationItemTypeId = json["destination_item_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_destinationItemTypeId = std::nullopt;
    }
    // Флаг двунаправленной связи
    if (json.contains("is_bidirectional") && !json["is_bidirectional"].is_null())
    {
        try
        {
            m_isBidirectional = json["is_bidirectional"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isBidirectional = std::nullopt;
    }
    // Название типа связи
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

    return success;
}

bool LinkType::isValid() const
{
    if (!m_sourceItemTypeId.has_value())
    {
        return false;
    }
    if (!m_destinationItemTypeId.has_value())
    {
        return false;
    }
    if (!m_isBidirectional.has_value())
    {
        return false;
    }
    if (!m_caption.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_caption.value().empty())
    {
        return false;
    }

    return true;
}

std::string LinkType::validationError() const
{
    if (!m_sourceItemTypeId.has_value())
    {
        return "Поле «source_item_type_id» является обязательным для заполнения";
    }
    if (!m_destinationItemTypeId.has_value())
    {
        return "Поле «destination_item_type_id» является обязательным для заполнения";
    }
    if (!m_isBidirectional.has_value())
    {
        return "Поле «is_bidirectional» является обязательным для заполнения";
    }
    if (!m_caption.has_value())
    {
        return "Поле «caption» является обязательным для заполнения";
    }

    if (m_caption.value().empty())
    {
        return "Поле «caption» не может быть пустой строкой";
    }

    return "";
}

bool LinkType::operator==(const LinkType& other) const
{
    return
        m_id == other.m_id
        && m_sourceItemTypeId == other.m_sourceItemTypeId
        && m_destinationItemTypeId == other.m_destinationItemTypeId
        && m_isBidirectional == other.m_isBidirectional
        && m_caption == other.m_caption
;
}

} // namespace dto