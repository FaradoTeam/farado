#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "phase.h"

namespace dto
{

Phase::Phase(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Phase::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор фазы
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор проекта
    if (m_projectId.has_value())
    {
        result["project_id"] = m_projectId.value();
    }
    // Название фазы
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание фазы
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }
    // Дата начала фазы
    if (m_beginDate.has_value())
    {
        result["begin_date"] = timePointToSeconds(m_beginDate.value());
    }
    // Дата окончания фазы
    if (m_endDate.has_value())
    {
        result["end_date"] = timePointToSeconds(m_endDate.value());
    }
    // Флаг архивации фазы
    if (m_isArchive.has_value())
    {
        result["is_archive"] = m_isArchive.value();
    }

    return result;
}

bool Phase::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор фазы
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
    // Идентификатор проекта
    if (json.contains("project_id") && !json["project_id"].is_null())
    {
        try
        {
            m_projectId = json["project_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_projectId = std::nullopt;
    }
    // Название фазы
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
    // Описание фазы
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
    // Дата начала фазы
    if (json.contains("begin_date") && !json["begin_date"].is_null())
    {
        try
        {
            auto timestamp = json["begin_date"].get<int64_t>();
            m_beginDate = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_beginDate = std::nullopt;
    }
    // Дата окончания фазы
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
    // Флаг архивации фазы
    if (json.contains("is_archive") && !json["is_archive"].is_null())
    {
        try
        {
            m_isArchive = json["is_archive"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isArchive = std::nullopt;
    }

    return success;
}

bool Phase::isValid() const
{
    if (!m_projectId.has_value())
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

std::string Phase::validationError() const
{
    if (!m_projectId.has_value())
    {
        return "Поле «project_id» является обязательным для заполнения";
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

bool Phase::operator==(const Phase& other) const
{
    return
        m_id == other.m_id
        && m_projectId == other.m_projectId
        && m_caption == other.m_caption
        && m_description == other.m_description
        && m_beginDate == other.m_beginDate
        && m_endDate == other.m_endDate
        && m_isArchive == other.m_isArchive
;
}

} // namespace dto