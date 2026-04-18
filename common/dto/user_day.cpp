#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "user_day.h"

namespace dto
{

UserDay::UserDay(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json UserDay::toJson() const
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
    // Дата (уникальна в паре с userId)
    if (m_date.has_value())
    {
        result["date"] = timePointToSeconds(m_date.value());
    }
    // Является ли день рабочим для пользователя
    if (m_isWorkDay.has_value())
    {
        result["is_work_day"] = m_isWorkDay.value();
    }
    // Время начала работы (если рабочий)
    if (m_beginWorkTime.has_value())
    {
        result["begin_work_time"] = m_beginWorkTime.value();
    }
    // Время окончания работы (если рабочий)
    if (m_endWorkTime.has_value())
    {
        result["end_work_time"] = m_endWorkTime.value();
    }
    // Длительность перерыва в минутах
    if (m_breakDuration.has_value())
    {
        result["break_duration"] = m_breakDuration.value();
    }
    // Причина (отпуск
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }

    return result;
}

bool UserDay::fromJson(const nlohmann::json& json)
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
    // Дата (уникальна в паре с userId)
    if (json.contains("date") && !json["date"].is_null())
    {
        try
        {
            auto timestamp = json["date"].get<int64_t>();
            m_date = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_date = std::nullopt;
    }
    // Является ли день рабочим для пользователя
    if (json.contains("is_work_day") && !json["is_work_day"].is_null())
    {
        try
        {
            m_isWorkDay = json["is_work_day"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isWorkDay = std::nullopt;
    }
    // Время начала работы (если рабочий)
    if (json.contains("begin_work_time") && !json["begin_work_time"].is_null())
    {
        try
        {
            m_beginWorkTime = json["begin_work_time"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_beginWorkTime = std::nullopt;
    }
    // Время окончания работы (если рабочий)
    if (json.contains("end_work_time") && !json["end_work_time"].is_null())
    {
        try
        {
            m_endWorkTime = json["end_work_time"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_endWorkTime = std::nullopt;
    }
    // Длительность перерыва в минутах
    if (json.contains("break_duration") && !json["break_duration"].is_null())
    {
        try
        {
            m_breakDuration = json["break_duration"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_breakDuration = std::nullopt;
    }
    // Причина (отпуск
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

bool UserDay::isValid() const
{
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_date.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string UserDay::validationError() const
{
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_date.has_value())
    {
        return "Поле «date» является обязательным для заполнения";
    }


    return "";
}

bool UserDay::operator==(const UserDay& other) const
{
    return
        m_id == other.m_id
        && m_userId == other.m_userId
        && m_date == other.m_date
        && m_isWorkDay == other.m_isWorkDay
        && m_beginWorkTime == other.m_beginWorkTime
        && m_endWorkTime == other.m_endWorkTime
        && m_breakDuration == other.m_breakDuration
        && m_description == other.m_description
;
}

} // namespace dto