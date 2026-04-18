#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "error_response.h"

namespace dto
{

ErrorResponse::ErrorResponse(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ErrorResponse::toJson() const
{
    nlohmann::json result;

    // Код ошибки HTTP
    if (m_code.has_value())
    {
        result["code"] = m_code.value();
    }
    // Сообщение об ошибке
    if (m_message.has_value())
    {
        result["message"] = m_message.value();
    }
    // Дополнительные детали ошибки
    if (m_details.has_value())
    {
        result["details"] = m_details.value();
    }

    return result;
}

bool ErrorResponse::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Код ошибки HTTP
    if (json.contains("code") && !json["code"].is_null())
    {
        try
        {
            m_code = json["code"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_code = std::nullopt;
    }
    // Сообщение об ошибке
    if (json.contains("message") && !json["message"].is_null())
    {
        try
        {
            m_message = json["message"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_message = std::nullopt;
    }
    // Дополнительные детали ошибки
    if (json.contains("details") && !json["details"].is_null())
    {
        try
        {
            m_details = json["details"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_details = std::nullopt;
    }

    return success;
}

bool ErrorResponse::isValid() const
{
    if (!m_code.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string ErrorResponse::validationError() const
{
    if (!m_code.has_value())
    {
        return "Поле «code» является обязательным для заполнения";
    }


    return "";
}

bool ErrorResponse::operator==(const ErrorResponse& other) const
{
    return
        m_code == other.m_code
        && m_message == other.m_message
        && m_details == other.m_details
;
}

} // namespace dto