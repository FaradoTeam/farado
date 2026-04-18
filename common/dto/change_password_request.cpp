#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "change_password_request.h"

namespace dto
{

ChangePasswordRequest::ChangePasswordRequest(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ChangePasswordRequest::toJson() const
{
    nlohmann::json result;

    // oldPassword
    if (m_oldPassword.has_value())
    {
        result["old_password"] = m_oldPassword.value();
    }
    // newPassword
    if (m_newPassword.has_value())
    {
        result["new_password"] = m_newPassword.value();
    }

    return result;
}

bool ChangePasswordRequest::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // oldPassword
    if (json.contains("old_password") && !json["old_password"].is_null())
    {
        try
        {
            m_oldPassword = json["old_password"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_oldPassword = std::nullopt;
    }
    // newPassword
    if (json.contains("new_password") && !json["new_password"].is_null())
    {
        try
        {
            m_newPassword = json["new_password"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_newPassword = std::nullopt;
    }

    return success;
}

bool ChangePasswordRequest::isValid() const
{
    if (!m_oldPassword.has_value())
    {
        return false;
    }
    if (!m_newPassword.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_oldPassword.value().empty())
    {
        return false;
    }
    if (m_newPassword.value().empty())
    {
        return false;
    }

    return true;
}

std::string ChangePasswordRequest::validationError() const
{
    if (!m_oldPassword.has_value())
    {
        return "Поле «old_password» является обязательным для заполнения";
    }
    if (!m_newPassword.has_value())
    {
        return "Поле «new_password» является обязательным для заполнения";
    }

    if (m_oldPassword.value().empty())
    {
        return "Поле «old_password» не может быть пустой строкой";
    }
    if (m_newPassword.value().empty())
    {
        return "Поле «new_password» не может быть пустой строкой";
    }

    return "";
}

bool ChangePasswordRequest::operator==(const ChangePasswordRequest& other) const
{
    return
        m_oldPassword == other.m_oldPassword
        && m_newPassword == other.m_newPassword
;
}

} // namespace dto