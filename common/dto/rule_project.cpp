#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "rule_project.h"

namespace dto
{

RuleProject::RuleProject(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json RuleProject::toJson() const
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
    // Идентификатор проекта
    if (m_projectId.has_value())
    {
        result["project_id"] = m_projectId.value();
    }
    // Право на просмотр элементов (items) проекта
    if (m_isReader.has_value())
    {
        result["is_reader"] = m_isReader.value();
    }
    // Право на создание и изменение элементов (items) проекта
    if (m_isWriter.has_value())
    {
        result["is_writer"] = m_isWriter.value();
    }
    // Право на создание и изменение подпроектов
    if (m_isProjectEditor.has_value())
    {
        result["is_project_editor"] = m_isProjectEditor.value();
    }
    // Право на создание и изменение фаз
    if (m_isPhaseEditor.has_value())
    {
        result["is_phase_editor"] = m_isPhaseEditor.value();
    }
    // Право на создание и изменение досок
    if (m_isBoardEditor.has_value())
    {
        result["is_board_editor"] = m_isBoardEditor.value();
    }

    return result;
}

bool RuleProject::fromJson(const nlohmann::json& json)
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
    // Идентификатор проекта
    if (json.contains("project_id") && !json["project_id"].is_null())
    {
        try
        {
            m_projectId = json["project_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_projectId = std::nullopt;
    }
    // Право на просмотр элементов (items) проекта
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
    // Право на создание и изменение элементов (items) проекта
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
    // Право на создание и изменение подпроектов
    if (json.contains("is_project_editor") && !json["is_project_editor"].is_null())
    {
        try
        {
            m_isProjectEditor = json["is_project_editor"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isProjectEditor = std::nullopt;
    }
    // Право на создание и изменение фаз
    if (json.contains("is_phase_editor") && !json["is_phase_editor"].is_null())
    {
        try
        {
            m_isPhaseEditor = json["is_phase_editor"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isPhaseEditor = std::nullopt;
    }
    // Право на создание и изменение досок
    if (json.contains("is_board_editor") && !json["is_board_editor"].is_null())
    {
        try
        {
            m_isBoardEditor = json["is_board_editor"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isBoardEditor = std::nullopt;
    }

    return success;
}

bool RuleProject::isValid() const
{
    if (!m_ruleId.has_value())
    {
        return false;
    }
    if (!m_projectId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string RuleProject::validationError() const
{
    if (!m_ruleId.has_value())
    {
        return "Поле «rule_id» является обязательным для заполнения";
    }
    if (!m_projectId.has_value())
    {
        return "Поле «project_id» является обязательным для заполнения";
    }


    return "";
}

bool RuleProject::operator==(const RuleProject& other) const
{
    return
        m_id == other.m_id
        && m_ruleId == other.m_ruleId
        && m_projectId == other.m_projectId
        && m_isReader == other.m_isReader
        && m_isWriter == other.m_isWriter
        && m_isProjectEditor == other.m_isProjectEditor
        && m_isPhaseEditor == other.m_isPhaseEditor
        && m_isBoardEditor == other.m_isBoardEditor
;
}

} // namespace dto