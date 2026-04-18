#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "private_message.h"

namespace dto
{

PrivateMessage::PrivateMessage(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json PrivateMessage::toJson() const
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
    // Идентификатор получателя
    if (m_receiverUserId.has_value())
    {
        result["receiver_user_id"] = m_receiverUserId.value();
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
    // Флаг прочтения
    if (m_isViewed.has_value())
    {
        result["is_viewed"] = m_isViewed.value();
    }

    return result;
}

bool PrivateMessage::fromJson(const nlohmann::json& json)
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
    // Идентификатор получателя
    if (json.contains("receiver_user_id") && !json["receiver_user_id"].is_null())
    {
        try
        {
            m_receiverUserId = json["receiver_user_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_receiverUserId = std::nullopt;
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
    // Флаг прочтения
    if (json.contains("is_viewed") && !json["is_viewed"].is_null())
    {
        try
        {
            m_isViewed = json["is_viewed"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isViewed = std::nullopt;
    }

    return success;
}

bool PrivateMessage::isValid() const
{
    if (!m_senderUserId.has_value())
    {
        return false;
    }
    if (!m_receiverUserId.has_value())
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

std::string PrivateMessage::validationError() const
{
    if (!m_senderUserId.has_value())
    {
        return "Поле «sender_user_id» является обязательным для заполнения";
    }
    if (!m_receiverUserId.has_value())
    {
        return "Поле «receiver_user_id» является обязательным для заполнения";
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

bool PrivateMessage::operator==(const PrivateMessage& other) const
{
    return
        m_id == other.m_id
        && m_senderUserId == other.m_senderUserId
        && m_receiverUserId == other.m_receiverUserId
        && m_creationTimestamp == other.m_creationTimestamp
        && m_content == other.m_content
        && m_isViewed == other.m_isViewed
;
}

} // namespace dto