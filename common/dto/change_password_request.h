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
 * @brief ChangePasswordRequest DTO
 */
class ChangePasswordRequest final
{
public:
    using Ptr = std::shared_ptr<ChangePasswordRequest>;
    using ConstPtr = std::shared_ptr<const ChangePasswordRequest>;

public:
    ChangePasswordRequest() = default;
    explicit ChangePasswordRequest(const nlohmann::json& json);

    // Поле: «oldPassword» 
    const std::optional<std::string>& oldPassword() const { return m_oldPassword; }
    bool hasOldPassword() const { return m_oldPassword.has_value(); }
    void setOldPassword(const std::string& value)
    {
        m_oldPassword = value;
    }
    void setOldPassword(std::string&& value)
    {
        m_oldPassword = std::move(value);
    }
    void clearOldPassword()
    {
        m_oldPassword = std::nullopt;
    }

    // Поле: «newPassword» 
    const std::optional<std::string>& newPassword() const { return m_newPassword; }
    bool hasNewPassword() const { return m_newPassword.has_value(); }
    void setNewPassword(const std::string& value)
    {
        m_newPassword = value;
    }
    void setNewPassword(std::string&& value)
    {
        m_newPassword = std::move(value);
    }
    void clearNewPassword()
    {
        m_newPassword = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ChangePasswordRequest& other) const;
    bool operator!=(const ChangePasswordRequest& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ChangePasswordRequest& dto);

private:
    std::optional<std::string> m_oldPassword;
    std::optional<std::string> m_newPassword;
};

inline std::ostream& operator<<(std::ostream& os, const ChangePasswordRequest& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto