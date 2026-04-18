#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "auth_request.h"

namespace dto
{

AuthRequest::AuthRequest(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json AuthRequest::toJson() const
{
    nlohmann::json result;

    // login
    if (m_login.has_value())
    {
        result["login"] = m_login.value();
    }
    // password
    if (m_password.has_value())
    {
        result["password"] = m_password.value();
    }

    return result;
}

bool AuthRequest::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // login
    if (json.contains("login") && !json["login"].is_null())
    {
        try
        {
            m_login = json["login"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_login = std::nullopt;
    }
    // password
    if (json.contains("password") && !json["password"].is_null())
    {
        try
        {
            m_password = json["password"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_password = std::nullopt;
    }

    return success;
}

bool AuthRequest::isValid() const
{
    if (!m_login.has_value())
    {
        return false;
    }
    if (!m_password.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_login.value().empty())
    {
        return false;
    }
    if (m_password.value().empty())
    {
        return false;
    }

    return true;
}

std::string AuthRequest::validationError() const
{
    if (!m_login.has_value())
    {
        return "Поле «login» является обязательным для заполнения";
    }
    if (!m_password.has_value())
    {
        return "Поле «password» является обязательным для заполнения";
    }

    if (m_login.value().empty())
    {
        return "Поле «login» не может быть пустой строкой";
    }
    if (m_password.value().empty())
    {
        return "Поле «password» не может быть пустой строкой";
    }

    return "";
}

bool AuthRequest::operator==(const AuthRequest& other) const
{
    return
        m_login == other.m_login
        && m_password == other.m_password
;
}

} // namespace dto