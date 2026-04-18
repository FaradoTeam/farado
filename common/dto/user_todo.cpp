#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "user_todo.h"

namespace dto
{

UserTodo::UserTodo(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json UserTodo::toJson() const
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
    // Плановая дата начала
    if (m_startDate.has_value())
    {
        result["start_date"] = timePointToSeconds(m_startDate.value());
    }
    // Плановая дата окончания
    if (m_endDate.has_value())
    {
        result["end_date"] = timePointToSeconds(m_endDate.value());
    }

    return result;
}

bool UserTodo::fromJson(const nlohmann::json& json)
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
    // Плановая дата начала
    if (json.contains("start_date") && !json["start_date"].is_null())
    {
        try
        {
            auto timestamp = json["start_date"].get<int64_t>();
            m_startDate = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_startDate = std::nullopt;
    }
    // Плановая дата окончания
    if (json.contains("end_date") && !json["end_date"].is_null())
    {
        try
        {
            auto timestamp = json["end_date"].get<int64_t>();
            m_endDate = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_endDate = std::nullopt;
    }

    return success;
}

bool UserTodo::isValid() const
{
    if (!m_itemId.has_value())
    {
        return false;
    }
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_startDate.has_value())
    {
        return false;
    }
    if (!m_endDate.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string UserTodo::validationError() const
{
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
    }
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_startDate.has_value())
    {
        return "Поле «start_date» является обязательным для заполнения";
    }
    if (!m_endDate.has_value())
    {
        return "Поле «end_date» является обязательным для заполнения";
    }


    return "";
}

bool UserTodo::operator==(const UserTodo& other) const
{
    return
        m_id == other.m_id
        && m_itemId == other.m_itemId
        && m_userId == other.m_userId
        && m_startDate == other.m_startDate
        && m_endDate == other.m_endDate
;
}

} // namespace dto