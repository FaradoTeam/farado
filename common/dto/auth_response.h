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
 * @brief AuthResponse DTO
 */
class AuthResponse final
{
public:
    using Ptr = std::shared_ptr<AuthResponse>;
    using ConstPtr = std::shared_ptr<const AuthResponse>;

public:
    AuthResponse() = default;
    explicit AuthResponse(const nlohmann::json& json);

    // Поле: «accessToken» 
    const std::optional<std::string>& accessToken() const { return m_accessToken; }
    bool hasAccessToken() const { return m_accessToken.has_value(); }
    void setAccessToken(const std::string& value)
    {
        m_accessToken = value;
    }
    void setAccessToken(std::string&& value)
    {
        m_accessToken = std::move(value);
    }
    void clearAccessToken()
    {
        m_accessToken = std::nullopt;
    }

    // Поле: «tokenType» 
    const std::string& tokenType() const { return m_tokenType; }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const AuthResponse& other) const;
    bool operator!=(const AuthResponse& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const AuthResponse& dto);

private:
    std::optional<std::string> m_accessToken;
    static inline const std::string m_tokenType = "Bearer";
};

inline std::ostream& operator<<(std::ostream& os, const AuthResponse& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto