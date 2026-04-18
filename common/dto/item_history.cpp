#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_history.h"

namespace dto
{

ItemHistory::ItemHistory(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemHistory::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор элемента
    if (m_itemId.has_value())
    {
        result["item_id"] = m_itemId.value();
    }
    // Идентификатор пользователя
    if (m_userId.has_value())
    {
        result["user_id"] = m_userId.value();
    }
    // Время изменения
    if (m_timestamp.has_value())
    {
        result["timestamp"] = timePointToSeconds(m_timestamp.value());
    }
    // JSON с изменениями (diff)
    if (m_diff.has_value())
    {
        result["diff"] = m_diff.value();
    }

    return result;
}

bool ItemHistory::fromJson(const nlohmann::json& json)
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
    // Время изменения
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
    // JSON с изменениями (diff)
    if (json.contains("diff") && !json["diff"].is_null())
    {
        try
        {
            m_diff = json["diff"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_diff = std::nullopt;
    }

    return success;
}

bool ItemHistory::isValid() const
{
    if (!m_itemId.has_value())
    {
        return false;
    }
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_timestamp.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string ItemHistory::validationError() const
{
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
    }
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_timestamp.has_value())
    {
        return "Поле «timestamp» является обязательным для заполнения";
    }


    return "";
}

bool ItemHistory::operator==(const ItemHistory& other) const
{
    return
        m_id == other.m_id
        && m_itemId == other.m_itemId
        && m_userId == other.m_userId
        && m_timestamp == other.m_timestamp
        && m_diff == other.m_diff
;
}

} // namespace dto