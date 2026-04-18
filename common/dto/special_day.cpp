#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "special_day.h"

namespace dto
{

SpecialDay::SpecialDay(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json SpecialDay::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Дата (уникальная)
    if (m_date.has_value())
    {
        result["date"] = timePointToSeconds(m_date.value());
    }
    // Является ли день рабочим
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

    return result;
}

bool SpecialDay::fromJson(const nlohmann::json& json)
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
    // Дата (уникальная)
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
    // Является ли день рабочим
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

    return success;
}

bool SpecialDay::isValid() const
{
    if (!m_date.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string SpecialDay::validationError() const
{
    if (!m_date.has_value())
    {
        return "Поле «date» является обязательным для заполнения";
    }


    return "";
}

bool SpecialDay::operator==(const SpecialDay& other) const
{
    return
        m_id == other.m_id
        && m_date == other.m_date
        && m_isWorkDay == other.m_isWorkDay
        && m_beginWorkTime == other.m_beginWorkTime
        && m_endWorkTime == other.m_endWorkTime
        && m_breakDuration == other.m_breakDuration
;
}

} // namespace dto