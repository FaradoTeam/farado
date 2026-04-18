#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "document.h"

namespace dto
{

Document::Document(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Document::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Название документа
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание документа
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }
    // Путь к файлу на диске
    if (m_path.has_value())
    {
        result["path"] = m_path.value();
    }
    // Исходное имя файла
    if (m_filename.has_value())
    {
        result["filename"] = m_filename.value();
    }
    // Размер файла в байтах
    if (m_size.has_value())
    {
        result["size"] = m_size.value();
    }
    // MIME-тип файла
    if (m_mimeType.has_value())
    {
        result["mime_type"] = m_mimeType.value();
    }
    // Время загрузки
    if (m_uploadedAt.has_value())
    {
        result["uploaded_at"] = timePointToSeconds(m_uploadedAt.value());
    }
    // Идентификатор пользователя
    if (m_uploadedByUserId.has_value())
    {
        result["uploaded_by_user_id"] = m_uploadedByUserId.value();
    }

    return result;
}

bool Document::fromJson(const nlohmann::json& json)
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
    // Название документа
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
    // Описание документа
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
    // Путь к файлу на диске
    if (json.contains("path") && !json["path"].is_null())
    {
        try
        {
            m_path = json["path"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_path = std::nullopt;
    }
    // Исходное имя файла
    if (json.contains("filename") && !json["filename"].is_null())
    {
        try
        {
            m_filename = json["filename"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_filename = std::nullopt;
    }
    // Размер файла в байтах
    if (json.contains("size") && !json["size"].is_null())
    {
        try
        {
            m_size = json["size"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_size = std::nullopt;
    }
    // MIME-тип файла
    if (json.contains("mime_type") && !json["mime_type"].is_null())
    {
        try
        {
            m_mimeType = json["mime_type"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_mimeType = std::nullopt;
    }
    // Время загрузки
    if (json.contains("uploaded_at") && !json["uploaded_at"].is_null())
    {
        try
        {
            auto timestamp = json["uploaded_at"].get<int64_t>();
            m_uploadedAt = secondsToTimePoint(timestamp);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_uploadedAt = std::nullopt;
    }
    // Идентификатор пользователя
    if (json.contains("uploaded_by_user_id") && !json["uploaded_by_user_id"].is_null())
    {
        try
        {
            m_uploadedByUserId = json["uploaded_by_user_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_uploadedByUserId = std::nullopt;
    }

    return success;
}

bool Document::isValid() const
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

std::string Document::validationError() const
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

bool Document::operator==(const Document& other) const
{
    return
        m_id == other.m_id
        && m_caption == other.m_caption
        && m_description == other.m_description
        && m_path == other.m_path
        && m_filename == other.m_filename
        && m_size == other.m_size
        && m_mimeType == other.m_mimeType
        && m_uploadedAt == other.m_uploadedAt
        && m_uploadedByUserId == other.m_uploadedByUserId
;
}

} // namespace dto