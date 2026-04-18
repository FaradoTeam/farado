#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "rule_state.h"

namespace dto
{

RuleState::RuleState(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json RuleState::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор правила
    if (m_ruleId.has_value())
    {
        result["rule_id"] = m_ruleId.value();
    }
    // Идентификатор целевого состояния
    if (m_stateId.has_value())
    {
        result["state_id"] = m_stateId.value();
    }
    // Разрешение на перевод элемента в это состояние
    if (m_isStateAllowed.has_value())
    {
        result["is_state_allowed"] = m_isStateAllowed.value();
    }

    return result;
}

bool RuleState::fromJson(const nlohmann::json& json)
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
    // Идентификатор правила
    if (json.contains("rule_id") && !json["rule_id"].is_null())
    {
        try
        {
            m_ruleId = json["rule_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_ruleId = std::nullopt;
    }
    // Идентификатор целевого состояния
    if (json.contains("state_id") && !json["state_id"].is_null())
    {
        try
        {
            m_stateId = json["state_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_stateId = std::nullopt;
    }
    // Разрешение на перевод элемента в это состояние
    if (json.contains("is_state_allowed") && !json["is_state_allowed"].is_null())
    {
        try
        {
            m_isStateAllowed = json["is_state_allowed"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isStateAllowed = std::nullopt;
    }

    return success;
}

bool RuleState::isValid() const
{
    if (!m_ruleId.has_value())
    {
        return false;
    }
    if (!m_stateId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string RuleState::validationError() const
{
    if (!m_ruleId.has_value())
    {
        return "Поле «rule_id» является обязательным для заполнения";
    }
    if (!m_stateId.has_value())
    {
        return "Поле «state_id» является обязательным для заполнения";
    }


    return "";
}

bool RuleState::operator==(const RuleState& other) const
{
    return
        m_id == other.m_id
        && m_ruleId == other.m_ruleId
        && m_stateId == other.m_stateId
        && m_isStateAllowed == other.m_isStateAllowed
;
}

} // namespace dto