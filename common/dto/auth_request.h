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
 * @brief AuthRequest DTO
 */
class AuthRequest final
{
public:
    using Ptr = std::shared_ptr<AuthRequest>;
    using ConstPtr = std::shared_ptr<const AuthRequest>;

public:
    AuthRequest() = default;
    explicit AuthRequest(const nlohmann::json& json);

    // Поле: «login» 
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

    // Поле: «password» 
    const std::optional<std::string>& password() const { return m_password; }
    bool hasPassword() const { return m_password.has_value(); }
    void setPassword(const std::string& value)
    {
        m_password = value;
    }
    void setPassword(std::string&& value)
    {
        m_password = std::move(value);
    }
    void clearPassword()
    {
        m_password = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const AuthRequest& other) const;
    bool operator!=(const AuthRequest& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const AuthRequest& dto);

private:
    std::optional<std::string> m_login;
    std::optional<std::string> m_password;
};

inline std::ostream& operator<<(std::ostream& os, const AuthRequest& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto