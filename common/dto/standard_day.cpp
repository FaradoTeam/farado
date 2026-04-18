#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "standard_day.h"

namespace dto
{

StandardDay::StandardDay(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json StandardDay::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Номер дня недели (0-6)
    if (m_weekDayNumber.has_value())
    {
        result["week_day_number"] = m_weekDayNumber.value();
    }
    // Порядковый номер недели (для сменного графика)
    if (m_weekOrder.has_value())
    {
        result["week_order"] = m_weekOrder.value();
    }
    // Является ли день рабочим
    if (m_isWorkDay.has_value())
    {
        result["is_work_day"] = m_isWorkDay.value();
    }
    // Время начала работы (HH:MM)
    if (m_beginWorkTime.has_value())
    {
        result["begin_work_time"] = m_beginWorkTime.value();
    }
    // Время окончания работы (HH:MM)
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

bool StandardDay::fromJson(const nlohmann::json& json)
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
    // Номер дня недели (0-6)
    if (json.contains("week_day_number") && !json["week_day_number"].is_null())
    {
        try
        {
            m_weekDayNumber = json["week_day_number"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_weekDayNumber = std::nullopt;
    }
    // Порядковый номер недели (для сменного графика)
    if (json.contains("week_order") && !json["week_order"].is_null())
    {
        try
        {
            m_weekOrder = json["week_order"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_weekOrder = std::nullopt;
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
    // Время начала работы (HH:MM)
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
    // Время окончания работы (HH:MM)
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

bool StandardDay::isValid() const
{
    if (!m_weekDayNumber.has_value())
    {
        return false;
    }
    if (!m_weekOrder.has_value())
    {
        return false;
    }
    if (!m_isWorkDay.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string StandardDay::validationError() const
{
    if (!m_weekDayNumber.has_value())
    {
        return "Поле «week_day_number» является обязательным для заполнения";
    }
    if (!m_weekOrder.has_value())
    {
        return "Поле «week_order» является обязательным для заполнения";
    }
    if (!m_isWorkDay.has_value())
    {
        return "Поле «is_work_day» является обязательным для заполнения";
    }


    return "";
}

bool StandardDay::operator==(const StandardDay& other) const
{
    return
        m_id == other.m_id
        && m_weekDayNumber == other.m_weekDayNumber
        && m_weekOrder == other.m_weekOrder
        && m_isWorkDay == other.m_isWorkDay
        && m_beginWorkTime == other.m_beginWorkTime
        && m_endWorkTime == other.m_endWorkTime
        && m_breakDuration == other.m_breakDuration
;
}

} // namespace dto