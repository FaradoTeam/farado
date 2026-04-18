#pragma once

#include <optional>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "common/helpers/print_helpers.h"


namespace dto
{

/**
 * @brief User DTO
 */
class User final
{
public:
    using Ptr = std::shared_ptr<User>;
    using ConstPtr = std::shared_ptr<const User>;

public:
    User() = default;
    explicit User(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор пользователя
    const std::optional<int64_t>& id() const { return m_id; }
    bool hasId() const { return m_id.has_value(); }
    void setId(const int64_t& value)
    {
        m_id = value;
    }
    void setId(int64_t&& value)
    {
        m_id = std::move(value);
    }
    void clearId()
    {
        m_id = std::nullopt;
    }

    // Поле: «login» Логин пользователя (уникальный)
    const std::optional<std::string>& login() const { return m_login; }
    bool hasLogin() const { return m_login.has_value(); }
    void setLogin(const std::string& value)
    {
        m_login = value;
    }
    void setLogin(std::string&& value)
    {
        m_login = std::move(value);
    }
    void clearLogin()
    {
        m_login = std::nullopt;
    }

    // Поле: «firstName» Имя пользователя
    const std::optional<std::string>& firstName() const { return m_firstName; }
    bool hasFirstName() const { return m_firstName.has_value(); }
    void setFirstName(const std::string& value)
    {
        m_firstName = value;
    }
    void setFirstName(std::string&& value)
    {
        m_firstName = std::move(value);
    }
    void clearFirstName()
    {
        m_firstName = std::nullopt;
    }

    // Поле: «middleName» Отчество пользователя
    const std::optional<std::string>& middleName() const { return m_middleName; }
    bool hasMiddleName() const { return m_middleName.has_value(); }
    void setMiddleName(const std::string& value)
    {
        m_middleName = value;
    }
    void setMiddleName(std::string&& value)
    {
        m_middleName = std::move(value);
    }
    void clearMiddleName()
    {
        m_middleName = std::nullopt;
    }

    // Поле: «lastName» Фамилия пользователя
    const std::optional<std::string>& lastName() const { return m_lastName; }
    bool hasLastName() const { return m_lastName.has_value(); }
    void setLastName(const std::string& value)
    {
        m_lastName = value;
    }
    void setLastName(std::string&& value)
    {
        m_lastName = std::move(value);
    }
    void clearLastName()
    {
        m_lastName = std::nullopt;
    }

    // Поле: «email» Email пользователя (уникальный)
    const std::optional<std::string>& email() const { return m_email; }
    bool hasEmail() const { return m_email.has_value(); }
    void setEmail(const std::string& value)
    {
        m_email = value;
    }
    void setEmail(std::string&& value)
    {
        m_email = std::move(value);
    }
    void clearEmail()
    {
        m_email = std::nullopt;
    }

    // Поле: «needChangePassword» Флаг необходимости смены пароля при следующем входе
    const std::optional<bool>& needChangePassword() const { return m_needChangePassword; }
    bool hasNeedChangePassword() const { return m_needChangePassword.has_value(); }
    void setNeedChangePassword(const bool& value)
    {
        m_needChangePassword = value;
    }
    void setNeedChangePassword(bool&& value)
    {
        m_needChangePassword = std::move(value);
    }
    void clearNeedChangePassword()
    {
        m_needChangePassword = std::nullopt;
    }

    // Поле: «isBlocked» Флаг блокировки пользователя (запрет входа)
    const std::optional<bool>& isBlocked() const { return m_isBlocked; }
    bool hasIsBlocked() const { return m_isBlocked.has_value(); }
    void setIsBlocked(const bool& value)
    {
        m_isBlocked = value;
    }
    void setIsBlocked(bool&& value)
    {
        m_isBlocked = std::move(value);
    }
    void clearIsBlocked()
    {
        m_isBlocked = std::nullopt;
    }

    // Поле: «isSuperAdmin» Флаг супер-администратора (имеет все права)
    const std::optional<bool>& isSuperAdmin() const { return m_isSuperAdmin; }
    bool hasIsSuperAdmin() const { return m_isSuperAdmin.has_value(); }
    void setIsSuperAdmin(const bool& value)
    {
        m_isSuperAdmin = value;
    }
    void setIsSuperAdmin(bool&& value)
    {
        m_isSuperAdmin = std::move(value);
    }
    void clearIsSuperAdmin()
    {
        m_isSuperAdmin = std::nullopt;
    }

    // Поле: «isHidden» Флаг скрытого пользователя (не отображается в списках)
    const std::optional<bool>& isHidden() const { return m_isHidden; }
    bool hasIsHidden() const { return m_isHidden.has_value(); }
    void setIsHidden(const bool& value)
    {
        m_isHidden = value;
    }
    void setIsHidden(bool&& value)
    {
        m_isHidden = std::move(value);
    }
    void clearIsHidden()
    {
        m_isHidden = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const User& other) const;
    bool operator!=(const User& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const User& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<std::string> m_login;
    std::optional<std::string> m_firstName;
    std::optional<std::string> m_middleName;
    std::optional<std::string> m_lastName;
    std::optional<std::string> m_email;
    std::optional<bool> m_needChangePassword;
    std::optional<bool> m_isBlocked;
    std::optional<bool> m_isSuperAdmin;
    std::optional<bool> m_isHidden;
};

inline std::ostream& operator<<(std::ostream& os, const User& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto