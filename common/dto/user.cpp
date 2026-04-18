#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "user.h"

namespace dto
{

User::User(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json User::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор пользователя
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Логин пользователя (уникальный)
    if (m_login.has_value())
    {
        result["login"] = m_login.value();
    }
    // Имя пользователя
    if (m_firstName.has_value())
    {
        result["first_name"] = m_firstName.value();
    }
    // Отчество пользователя
    if (m_middleName.has_value())
    {
        result["middle_name"] = m_middleName.value();
    }
    // Фамилия пользователя
    if (m_lastName.has_value())
    {
        result["last_name"] = m_lastName.value();
    }
    // Email пользователя (уникальный)
    if (m_email.has_value())
    {
        result["email"] = m_email.value();
    }
    // Флаг необходимости смены пароля при следующем входе
    if (m_needChangePassword.has_value())
    {
        result["need_change_password"] = m_needChangePassword.value();
    }
    // Флаг блокировки пользователя (запрет входа)
    if (m_isBlocked.has_value())
    {
        result["is_blocked"] = m_isBlocked.value();
    }
    // Флаг супер-администратора (имеет все права)
    if (m_isSuperAdmin.has_value())
    {
        result["is_super_admin"] = m_isSuperAdmin.value();
    }
    // Флаг скрытого пользователя (не отображается в списках)
    if (m_isHidden.has_value())
    {
        result["is_hidden"] = m_isHidden.value();
    }

    return result;
}

bool User::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор пользователя
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
    // Логин пользователя (уникальный)
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
    // Имя пользователя
    if (json.contains("first_name") && !json["first_name"].is_null())
    {
        try
        {
            m_firstName = json["first_name"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_firstName = std::nullopt;
    }
    // Отчество пользователя
    if (json.contains("middle_name") && !json["middle_name"].is_null())
    {
        try
        {
            m_middleName = json["middle_name"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_middleName = std::nullopt;
    }
    // Фамилия пользователя
    if (json.contains("last_name") && !json["last_name"].is_null())
    {
        try
        {
            m_lastName = json["last_name"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_lastName = std::nullopt;
    }
    // Email пользователя (уникальный)
    if (json.contains("email") && !json["email"].is_null())
    {
        try
        {
            m_email = json["email"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_email = std::nullopt;
    }
    // Флаг необходимости смены пароля при следующем входе
    if (json.contains("need_change_password") && !json["need_change_password"].is_null())
    {
        try
        {
            m_needChangePassword = json["need_change_password"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_needChangePassword = std::nullopt;
    }
    // Флаг блокировки пользователя (запрет входа)
    if (json.contains("is_blocked") && !json["is_blocked"].is_null())
    {
        try
        {
            m_isBlocked = json["is_blocked"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isBlocked = std::nullopt;
    }
    // Флаг супер-администратора (имеет все права)
    if (json.contains("is_super_admin") && !json["is_super_admin"].is_null())
    {
        try
        {
            m_isSuperAdmin = json["is_super_admin"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isSuperAdmin = std::nullopt;
    }
    // Флаг скрытого пользователя (не отображается в списках)
    if (json.contains("is_hidden") && !json["is_hidden"].is_null())
    {
        try
        {
            m_isHidden = json["is_hidden"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isHidden = std::nullopt;
    }

    return success;
}

bool User::isValid() const
{
    if (!m_login.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_login.value().empty())
    {
        return false;
    }

    return true;
}

std::string User::validationError() const
{
    if (!m_login.has_value())
    {
        return "Поле «login» является обязательным для заполнения";
    }

    if (m_login.value().empty())
    {
        return "Поле «login» не может быть пустой строкой";
    }

    return "";
}

bool User::operator==(const User& other) const
{
    return
        m_id == other.m_id
        && m_login == other.m_login
        && m_firstName == other.m_firstName
        && m_middleName == other.m_middleName
        && m_lastName == other.m_lastName
        && m_email == other.m_email
        && m_needChangePassword == other.m_needChangePassword
        && m_isBlocked == other.m_isBlocked
        && m_isSuperAdmin == other.m_isSuperAdmin
        && m_isHidden == other.m_isHidden
;
}

} // namespace dto