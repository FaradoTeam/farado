#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "comment.h"

namespace dto
{

Comment::Comment(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Comment::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор автора
    if (m_userId.has_value())
    {
        result["user_id"] = m_userId.value();
    }
    // Идентификатор элемента
    if (m_itemId.has_value())
    {
        result["item_id"] = m_itemId.value();
    }
    // Время создания
    if (m_createdAt.has_value())
    {
        result["created_at"] = timePointToSeconds(m_createdAt.value());
    }
    // Текст комментария
    if (m_content.has_value())
    {
        result["content"] = m_content.value();
    }

    return result;
}

bool Comment::fromJson(const nlohmann::json& json)
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
    // Идентификатор автора
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
    // Время создания
    if (json.contains("created_at") && !json["created_at"].is_null())
    {
        try
        {
            auto timestamp = json["created_at"].get<int64_t>();
            m_createdAt = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_createdAt = std::nullopt;
    }
    // Текст комментария
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

bool Comment::isValid() const
{
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_itemId.has_value())
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

std::string Comment::validationError() const
{
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
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

bool Comment::operator==(const Comment& other) const
{
    return
        m_id == other.m_id
        && m_userId == other.m_userId
        && m_itemId == other.m_itemId
        && m_createdAt == other.m_createdAt
        && m_content == other.m_content
;
}

} // namespace dto