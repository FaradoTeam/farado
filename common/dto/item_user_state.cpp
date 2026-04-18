#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_user_state.h"

namespace dto
{

ItemUserState::ItemUserState(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemUserState::toJson() const
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
    // Идентификатор состояния
    if (m_stateId.has_value())
    {
        result["state_id"] = m_stateId.value();
    }
    // Комментарий к переходу
    if (m_comment.has_value())
    {
        result["comment"] = m_comment.value();
    }
    // Время перехода
    if (m_timestamp.has_value())
    {
        result["timestamp"] = timePointToSeconds(m_timestamp.value());
    }

    return result;
}

bool ItemUserState::fromJson(const nlohmann::json& json)
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
    // Идентификатор состояния
    if (json.contains("state_id") && !json["state_id"].is_null())
    {
        try
        {
            m_stateId = json["state_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_stateId = std::nullopt;
    }
    // Комментарий к переходу
    if (json.contains("comment") && !json["comment"].is_null())
    {
        try
        {
            m_comment = json["comment"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_comment = std::nullopt;
    }
    // Время перехода
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

    return success;
}

bool ItemUserState::isValid() const
{
    if (!m_itemId.has_value())
    {
        return false;
    }
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_stateId.has_value())
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

std::string ItemUserState::validationError() const
{
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
    }
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_stateId.has_value())
    {
        return "Поле «state_id» является обязательным для заполнения";
    }
    if (!m_timestamp.has_value())
    {
        return "Поле «timestamp» является обязательным для заполнения";
    }


    return "";
}

bool ItemUserState::operator==(const ItemUserState& other) const
{
    return
        m_id == other.m_id
        && m_itemId == other.m_itemId
        && m_userId == other.m_userId
        && m_stateId == other.m_stateId
        && m_comment == other.m_comment
        && m_timestamp == other.m_timestamp
;
}

} // namespace dto