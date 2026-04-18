#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "project.h"

namespace dto
{

Project::Project(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Project::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор проекта
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор родительского проекта (null для корневых)
    if (m_parentId.has_value())
    {
        result["parent_id"] = m_parentId.value();
    }
    // Название проекта
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание проекта
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }
    // Дата и время создания
    if (m_createdAt.has_value())
    {
        result["created_at"] = timePointToSeconds(m_createdAt.value());
    }
    // Дата и время последнего обновления
    if (m_updatedAt.has_value())
    {
        result["updated_at"] = timePointToSeconds(m_updatedAt.value());
    }
    // Флаг архивации (мягкое удаление)
    if (m_isArchive.has_value())
    {
        result["is_archive"] = m_isArchive.value();
    }

    return result;
}

bool Project::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // Уникальный идентификатор проекта
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
    // Идентификатор родительского проекта (null для корневых)
    if (json.contains("parent_id") && !json["parent_id"].is_null())
    {
        try
        {
            m_parentId = json["parent_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_parentId = std::nullopt;
    }
    // Название проекта
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
    // Описание проекта
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
    // Дата и время создания
    if (json.contains("created_at") && !json["created_at"].is_null())
    {
        try
        {
            auto timestamp = json["created_at"].get<int64_t>();
            m_createdAt = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_createdAt = std::nullopt;
    }
    // Дата и время последнего обновления
    if (json.contains("updated_at") && !json["updated_at"].is_null())
    {
        try
        {
            auto timestamp = json["updated_at"].get<int64_t>();
            m_updatedAt = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_updatedAt = std::nullopt;
    }
    // Флаг архивации (мягкое удаление)
    if (json.contains("is_archive") && !json["is_archive"].is_null())
    {
        try
        {
            m_isArchive = json["is_archive"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isArchive = std::nullopt;
    }

    return success;
}

bool Project::isValid() const
{
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

std::string Project::validationError() const
{
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

bool Project::operator==(const Project& other) const
{
    return
        m_id == other.m_id
        && m_parentId == other.m_parentId
        && m_caption == other.m_caption
        && m_description == other.m_description
        && m_createdAt == other.m_createdAt
        && m_updatedAt == other.m_updatedAt
        && m_isArchive == other.m_isArchive
;
}

} // namespace dto