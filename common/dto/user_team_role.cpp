#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "user_team_role.h"

namespace dto
{

UserTeamRole::UserTeamRole(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json UserTeamRole::toJson() const
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
    // Идентификатор команды
    if (m_teamId.has_value())
    {
        result["team_id"] = m_teamId.value();
    }
    // Идентификатор роли
    if (m_roleId.has_value())
    {
        result["role_id"] = m_roleId.value();
    }

    return result;
}

bool UserTeamRole::fromJson(const nlohmann::json& json)
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
    // Идентификатор роли
    if (json.contains("role_id") && !json["role_id"].is_null())
    {
        try
        {
            m_roleId = json["role_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_roleId = std::nullopt;
    }

    return success;
}

bool UserTeamRole::isValid() const
{
    if (!m_userId.has_value())
    {
        return false;
    }
    if (!m_teamId.has_value())
    {
        return false;
    }
    if (!m_roleId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string UserTeamRole::validationError() const
{
    if (!m_userId.has_value())
    {
        return "Поле «user_id» является обязательным для заполнения";
    }
    if (!m_teamId.has_value())
    {
        return "Поле «team_id» является обязательным для заполнения";
    }
    if (!m_roleId.has_value())
    {
        return "Поле «role_id» является обязательным для заполнения";
    }


    return "";
}

bool UserTeamRole::operator==(const UserTeamRole& other) const
{
    return
        m_id == other.m_id
        && m_userId == other.m_userId
        && m_teamId == other.m_teamId
        && m_roleId == other.m_roleId
;
}

} // namespace dto