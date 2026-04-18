#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "rule_item_type.h"

namespace dto
{

RuleItemType::RuleItemType(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json RuleItemType::toJson() const
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
    // Идентификатор типа элемента
    if (m_itemTypeId.has_value())
    {
        result["item_type_id"] = m_itemTypeId.value();
    }
    // Право на чтение элементов данного типа
    if (m_isReader.has_value())
    {
        result["is_reader"] = m_isReader.value();
    }
    // Право на создание и изменение элементов данного типа
    if (m_isWriter.has_value())
    {
        result["is_writer"] = m_isWriter.value();
    }

    return result;
}

bool RuleItemType::fromJson(const nlohmann::json& json)
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
    // Идентификатор типа элемента
    if (json.contains("item_type_id") && !json["item_type_id"].is_null())
    {
        try
        {
            m_itemTypeId = json["item_type_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_itemTypeId = std::nullopt;
    }
    // Право на чтение элементов данного типа
    if (json.contains("is_reader") && !json["is_reader"].is_null())
    {
        try
        {
            m_isReader = json["is_reader"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isReader = std::nullopt;
    }
    // Право на создание и изменение элементов данного типа
    if (json.contains("is_writer") && !json["is_writer"].is_null())
    {
        try
        {
            m_isWriter = json["is_writer"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isWriter = std::nullopt;
    }

    return success;
}

bool RuleItemType::isValid() const
{
    if (!m_ruleId.has_value())
    {
        return false;
    }
    if (!m_itemTypeId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string RuleItemType::validationError() const
{
    if (!m_ruleId.has_value())
    {
        return "Поле «rule_id» является обязательным для заполнения";
    }
    if (!m_itemTypeId.has_value())
    {
        return "Поле «item_type_id» является обязательным для заполнения";
    }


    return "";
}

bool RuleItemType::operator==(const RuleItemType& other) const
{
    return
        m_id == other.m_id
        && m_ruleId == other.m_ruleId
        && m_itemTypeId == other.m_itemTypeId
        && m_isReader == other.m_isReader
        && m_isWriter == other.m_isWriter
;
}

} // namespace dto