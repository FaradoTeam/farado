#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "role.h"

namespace dto
{

Role::Role(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Role::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор роли
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Название роли
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание роли
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }

    return result;
}

bool Role::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор роли
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
    // Название роли
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
    // Описание роли
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

    return success;
}

bool Role::isValid() const
{
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

std::string Role::validationError() const
{
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

bool Role::operator==(const Role& other) const
{
    return
        m_id == other.m_id
        && m_caption == other.m_caption
        && m_description == other.m_description
;
}

} // namespace dto