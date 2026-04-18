#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "rule.h"

namespace dto
{

Rule::Rule(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Rule::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор правила
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор роли
    if (m_roleId.has_value())
    {
        result["role_id"] = m_roleId.value();
    }
    // Право на создание корневых проектов
    if (m_isRootProjectCreator.has_value())
    {
        result["is_root_project_creator"] = m_isRootProjectCreator.value();
    }

    return result;
}

bool Rule::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор правила
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
    // Идентификатор роли
    if (json.contains("role_id") && !json["role_id"].is_null())
    {
        try
        {
            m_roleId = json["role_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_roleId = std::nullopt;
    }
    // Право на создание корневых проектов
    if (json.contains("is_root_project_creator") && !json["is_root_project_creator"].is_null())
    {
        try
        {
            m_isRootProjectCreator = json["is_root_project_creator"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isRootProjectCreator = std::nullopt;
    }

    return success;
}

bool Rule::isValid() const
{
    if (!m_roleId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string Rule::validationError() const
{
    if (!m_roleId.has_value())
    {
        return "Поле «role_id» является обязательным для заполнения";
    }


    return "";
}

bool Rule::operator==(const Rule& other) const
{
    return
        m_id == other.m_id
        && m_roleId == other.m_roleId
        && m_isRootProjectCreator == other.m_isRootProjectCreator
;
}

} // namespace dto