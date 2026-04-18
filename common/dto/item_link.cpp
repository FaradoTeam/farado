#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_link.h"

namespace dto
{

ItemLink::ItemLink(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemLink::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор типа связи
    if (m_linkTypeId.has_value())
    {
        result["link_type_id"] = m_linkTypeId.value();
    }
    // Исходный элемент
    if (m_sourceItemId.has_value())
    {
        result["source_item_id"] = m_sourceItemId.value();
    }
    // Целевой элемент
    if (m_destinationItemId.has_value())
    {
        result["destination_item_id"] = m_destinationItemId.value();
    }

    return result;
}

bool ItemLink::fromJson(const nlohmann::json& json)
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
    // Идентификатор типа связи
    if (json.contains("link_type_id") && !json["link_type_id"].is_null())
    {
        try
        {
            m_linkTypeId = json["link_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_linkTypeId = std::nullopt;
    }
    // Исходный элемент
    if (json.contains("source_item_id") && !json["source_item_id"].is_null())
    {
        try
        {
            m_sourceItemId = json["source_item_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_sourceItemId = std::nullopt;
    }
    // Целевой элемент
    if (json.contains("destination_item_id") && !json["destination_item_id"].is_null())
    {
        try
        {
            m_destinationItemId = json["destination_item_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_destinationItemId = std::nullopt;
    }

    return success;
}

bool ItemLink::isValid() const
{
    if (!m_linkTypeId.has_value())
    {
        return false;
    }
    if (!m_sourceItemId.has_value())
    {
        return false;
    }
    if (!m_destinationItemId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string ItemLink::validationError() const
{
    if (!m_linkTypeId.has_value())
    {
        return "Поле «link_type_id» является обязательным для заполнения";
    }
    if (!m_sourceItemId.has_value())
    {
        return "Поле «source_item_id» является обязательным для заполнения";
    }
    if (!m_destinationItemId.has_value())
    {
        return "Поле «destination_item_id» является обязательным для заполнения";
    }


    return "";
}

bool ItemLink::operator==(const ItemLink& other) const
{
    return
        m_id == other.m_id
        && m_linkTypeId == other.m_linkTypeId
        && m_sourceItemId == other.m_sourceItemId
        && m_destinationItemId == other.m_destinationItemId
;
}

} // namespace dto