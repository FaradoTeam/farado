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
 * @brief ErrorResponse DTO
 */
class ErrorResponse final
{
public:
    using Ptr = std::shared_ptr<ErrorResponse>;
    using ConstPtr = std::shared_ptr<const ErrorResponse>;

public:
    ErrorResponse() = default;
    explicit ErrorResponse(const nlohmann::json& json);

    // Поле: «code» Код ошибки HTTP
    const std::optional<int64_t>& code() const { return m_code; }
    bool hasCode() const { return m_code.has_value(); }
    void setCode(const int64_t& value)
    {
        m_code = value;
    }
    void setCode(int64_t&& value)
    {
        m_code = std::move(value);
    }
    void clearCode()
    {
        m_code = std::nullopt;
    }

    // Поле: «message» Сообщение об ошибке
    const std::optional<std::string>& message() const { return m_message; }
    bool hasMessage() const { return m_message.has_value(); }
    void setMessage(const std::string& value)
    {
        m_message = value;
    }
    void setMessage(std::string&& value)
    {
        m_message = std::move(value);
    }
    void clearMessage()
    {
        m_message = std::nullopt;
    }

    // Поле: «details» Дополнительные детали ошибки
    const std::optional<std::string>& details() const { return m_details; }
    bool hasDetails() const { return m_details.has_value(); }
    void setDetails(const std::string& value)
    {
        m_details = value;
    }
    void setDetails(std::string&& value)
    {
        m_details = std::move(value);
    }
    void clearDetails()
    {
        m_details = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ErrorResponse& other) const;
    bool operator!=(const ErrorResponse& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ErrorResponse& dto);

private:
    std::optional<int64_t> m_code;
    std::optional<std::string> m_message;
    std::optional<std::string> m_details;
};

inline std::ostream& operator<<(std::ostream& os, const ErrorResponse& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto