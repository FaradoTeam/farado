#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "project_team.h"

namespace dto
{

ProjectTeam::ProjectTeam(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ProjectTeam::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор проекта
    if (m_projectId.has_value())
    {
        result["project_id"] = m_projectId.value();
    }
    // Идентификатор команды
    if (m_teamId.has_value())
    {
        result["team_id"] = m_teamId.value();
    }

    return result;
}

bool ProjectTeam::fromJson(const nlohmann::json& json)
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
    // Идентификатор команды
    if (json.contains("team_id") && !json["team_id"].is_null())
    {
        try
        {
            m_teamId = json["team_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_teamId = std::nullopt;
    }

    return success;
}

bool ProjectTeam::isValid() const
{
    if (!m_projectId.has_value())
    {
        return false;
    }
    if (!m_teamId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string ProjectTeam::validationError() const
{
    if (!m_projectId.has_value())
    {
        return "Поле «project_id» является обязательным для заполнения";
    }
    if (!m_teamId.has_value())
    {
        return "Поле «team_id» является обязательным для заполнения";
    }


    return "";
}

bool ProjectTeam::operator==(const ProjectTeam& other) const
{
    return
        m_id == other.m_id
        && m_projectId == other.m_projectId
        && m_teamId == other.m_teamId
;
}

} // namespace dto