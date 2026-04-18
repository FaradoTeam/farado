#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "user_action.h"

namespace dto
{

UserAction::UserAction(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json UserAction::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор пользователя
    if (m_userId.has_value())
    {
        result["user_id"] = m_userId.value();
    }
    // Время действия
    if (m_timestamp.has_value())
    {
        result["timestamp"] = timePointToSeconds(m_timestamp.value());
    }
    // Краткое описание действия
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Подробное описание (контекст)
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }

    return result;
}

bool UserAction::fromJson(const nlohmann::json& json)
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
    // Идентификатор пользователя
    if (json.contains("user_id") && !json["user_id"].is_null())
    {
        try
        {
            m_userId = json["user_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_userId = std::nullopt;
    }
    // Время действия
    if (json.contains("timestamp") && !json["timestamp"].is_null())
    {
        try
        {
            auto timestamp = json["timestamp"].get<int64_t>();
            m_timestamp = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_timestamp = std::nullopt;
    }
    // Краткое описание действия
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
    // Подробное описание (контекст)
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

bool UserAction::isValid() const
{
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_timestamp.has_value())
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

std::string UserAction::validationError() const
{
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_timestamp.has_value())
    {
        return "Поле «timestamp» является обязательным для заполнения";
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

bool UserAction::operator==(const UserAction& other) const
{
    return
        m_id == other.m_id
        && m_userId == other.m_userId
        && m_timestamp == other.m_timestamp
        && m_caption == other.m_caption
        && m_description == other.m_description
;
}

} // namespace dto