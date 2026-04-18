#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "role_menu_item.h"

namespace dto
{

RoleMenuItem::RoleMenuItem(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json RoleMenuItem::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор роли
    if (m_roleId.has_value())
    {
        result["role_id"] = m_roleId.value();
    }
    // Название пункта меню
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Ссылка (URL)
    if (m_link.has_value())
    {
        result["link"] = m_link.value();
    }
    // Значок пункта меню
    if (m_icon.has_value())
    {
        result["icon"] = m_icon.value();
    }

    return result;
}

bool RoleMenuItem::fromJson(const nlohmann::json& json)
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
    // Идентификатор роли
    if (json.contains("role_id") && !json["role_id"].is_null())
    {
        try
        {
            m_roleId = json["role_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_roleId = std::nullopt;
    }
    // Название пункта меню
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
    // Ссылка (URL)
    if (json.contains("link") && !json["link"].is_null())
    {
        try
        {
            m_link = json["link"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_link = std::nullopt;
    }
    // Значок пункта меню
    if (json.contains("icon") && !json["icon"].is_null())
    {
        try
        {
            m_icon = json["icon"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_icon = std::nullopt;
    }

    return success;
}

bool RoleMenuItem::isValid() const
{
    if (!m_roleId.has_value())
    {
        return false;
    }
    if (!m_caption.has_value())
    {
        return false;
    }
    if (!m_link.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_caption.value().empty())
    {
        return false;
    }
    if (m_link.value().empty())
    {
        return false;
    }

    return true;
}

std::string RoleMenuItem::validationError() const
{
    if (!m_roleId.has_value())
    {
        return "Поле «role_id» является обязательным для заполнения";
    }
    if (!m_caption.has_value())
    {
        return "Поле «caption» является обязательным для заполнения";
    }
    if (!m_link.has_value())
    {
        return "Поле «link» является обязательным для заполнения";
    }

    if (m_caption.value().empty())
    {
        return "Поле «caption» не может быть пустой строкой";
    }
    if (m_link.value().empty())
    {
        return "Поле «link» не может быть пустой строкой";
    }

    return "";
}

bool RoleMenuItem::operator==(const RoleMenuItem& other) const
{
    return
        m_id == other.m_id
        && m_roleId == other.m_roleId
        && m_caption == other.m_caption
        && m_link == other.m_link
        && m_icon == other.m_icon
;
}

} // namespace dto