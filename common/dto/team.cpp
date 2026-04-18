#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "team.h"

namespace dto
{

Team::Team(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Team::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор команды
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Название команды
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание команды
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }

    return result;
}

bool Team::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор команды
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
    // Название команды
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
    // Описание команды
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

bool Team::isValid() const
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

std::string Team::validationError() const
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

bool Team::operator==(const Team& other) const
{
    return
        m_id == other.m_id
        && m_caption == other.m_caption
        && m_description == other.m_description
;
}

} // namespace dto