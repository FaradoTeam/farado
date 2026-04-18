#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "board.h"

namespace dto
{

Board::Board(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Board::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор рабочего процесса
    if (m_workflowId.has_value())
    {
        result["workflow_id"] = m_workflowId.value();
    }
    // Идентификатор проекта
    if (m_projectId.has_value())
    {
        result["project_id"] = m_projectId.value();
    }
    // Идентификатор фазы
    if (m_phaseId.has_value())
    {
        result["phase_id"] = m_phaseId.value();
    }
    // Название доски
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание доски
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }

    return result;
}

bool Board::fromJson(const nlohmann::json& json)
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
    // Идентификатор рабочего процесса
    if (json.contains("workflow_id") && !json["workflow_id"].is_null())
    {
        try
        {
            m_workflowId = json["workflow_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_workflowId = std::nullopt;
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
    // Идентификатор фазы
    if (json.contains("phase_id") && !json["phase_id"].is_null())
    {
        try
        {
            m_phaseId = json["phase_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_phaseId = std::nullopt;
    }
    // Название доски
    if (json.contains("caption") && !json["caption"].is_null())
    {
        try
        {
            m_caption = json["caption"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_caption = std::nullopt;
    }
    // Описание доски
    if (json.contains("description") && !json["description"].is_null())
    {
        try
        {
            m_description = json["description"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_description = std::nullopt;
    }

    return success;
}

bool Board::isValid() const
{
    if (!m_workflowId.has_value())
    {
        return false;
    }
    if (!m_caption.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_caption.value().empty())
    {
        return false;
    }

    return true;
}

std::string Board::validationError() const
{
    if (!m_workflowId.has_value())
    {
        return "Поле «workflow_id» является обязательным для заполнения";
    }
    if (!m_caption.has_value())
    {
        return "Поле «caption» является обязательным для заполнения";
    }

    if (m_caption.value().empty())
    {
        return "Поле «caption» не может быть пустой строкой";
    }

    return "";
}

bool Board::operator==(const Board& other) const
{
    return
        m_id == other.m_id
        && m_workflowId == other.m_workflowId
        && m_projectId == other.m_projectId
        && m_phaseId == other.m_phaseId
        && m_caption == other.m_caption
        && m_description == other.m_description
;
}

} // namespace dto