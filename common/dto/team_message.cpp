#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "team_message.h"

namespace dto
{

TeamMessage::TeamMessage(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json TeamMessage::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор отправителя
    if (m_senderUserId.has_value())
    {
        result["sender_user_id"] = m_senderUserId.value();
    }
    // Идентификатор команды
    if (m_teamId.has_value())
    {
        result["team_id"] = m_teamId.value();
    }
    // Время отправки
    if (m_creationTimestamp.has_value())
    {
        result["creation_timestamp"] = timePointToSeconds(m_creationTimestamp.value());
    }
    // Текст сообщения
    if (m_content.has_value())
    {
        result["content"] = m_content.value();
    }

    return result;
}

bool TeamMessage::fromJson(const nlohmann::json& json)
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
    // Идентификатор отправителя
    if (json.contains("sender_user_id") && !json["sender_user_id"].is_null())
    {
        try
        {
            m_senderUserId = json["sender_user_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_senderUserId = std::nullopt;
    }
    // Идентификатор команды
    if (json.contains("team_id") && !json["team_id"].is_null())
    {
        try
        {
            m_teamId = json["team_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_teamId = std::nullopt;
    }
    // Время отправки
    if (json.contains("creation_timestamp") && !json["creation_timestamp"].is_null())
    {
        try
        {
            auto timestamp = json["creation_timestamp"].get<int64_t>();
            m_creationTimestamp = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_creationTimestamp = std::nullopt;
    }
    // Текст сообщения
    if (json.contains("content") && !json["content"].is_null())
    {
        try
        {
            m_content = json["content"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_content = std::nullopt;
    }

    return success;
}

bool TeamMessage::isValid() const
{
    if (!m_senderUserId.has_value())
    {
        return false;
    }
    if (!m_teamId.has_value())
    {
        return false;
    }
    if (!m_creationTimestamp.has_value())
    {
        return false;
    }
    if (!m_content.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_content.value().empty())
    {
        return false;
    }

    return true;
}

std::string TeamMessage::validationError() const
{
    if (!m_senderUserId.has_value())
    {
        return "Поле «sender_user_id» является обязательным для заполнения";
    }
    if (!m_teamId.has_value())
    {
        return "Поле «team_id» является обязательным для заполнения";
    }
    if (!m_creationTimestamp.has_value())
    {
        return "Поле «creation_timestamp» является обязательным для заполнения";
    }
    if (!m_content.has_value())
    {
        return "Поле «content» является обязательным для заполнения";
    }

    if (m_content.value().empty())
    {
        return "Поле «content» не может быть пустой строкой";
    }

    return "";
}

bool TeamMessage::operator==(const TeamMessage& other) const
{
    return
        m_id == other.m_id
        && m_senderUserId == other.m_senderUserId
        && m_teamId == other.m_teamId
        && m_creationTimestamp == other.m_creationTimestamp
        && m_content == other.m_content
;
}

} // namespace dto