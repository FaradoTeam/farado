#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "edge.h"

namespace dto
{

Edge::Edge(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Edge::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Начальное состояние
    if (m_beginStateId.has_value())
    {
        result["begin_state_id"] = m_beginStateId.value();
    }
    // Конечное состояние
    if (m_endStateId.has_value())
    {
        result["end_state_id"] = m_endStateId.value();
    }

    return result;
}

bool Edge::fromJson(const nlohmann::json& json)
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
    // Начальное состояние
    if (json.contains("begin_state_id") && !json["begin_state_id"].is_null())
    {
        try
        {
            m_beginStateId = json["begin_state_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_beginStateId = std::nullopt;
    }
    // Конечное состояние
    if (json.contains("end_state_id") && !json["end_state_id"].is_null())
    {
        try
        {
            m_endStateId = json["end_state_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_endStateId = std::nullopt;
    }

    return success;
}

bool Edge::isValid() const
{
    if (!m_beginStateId.has_value())
    {
        return false;
    }
    if (!m_endStateId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string Edge::validationError() const
{
    if (!m_beginStateId.has_value())
    {
        return "Поле «begin_state_id» является обязательным для заполнения";
    }
    if (!m_endStateId.has_value())
    {
        return "Поле «end_state_id» является обязательным для заполнения";
    }


    return "";
}

bool Edge::operator==(const Edge& other) const
{
    return
        m_id == other.m_id
        && m_beginStateId == other.m_beginStateId
        && m_endStateId == other.m_endStateId
;
}

} // namespace dto