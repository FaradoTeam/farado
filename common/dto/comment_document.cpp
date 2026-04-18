#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "comment_document.h"

namespace dto
{

CommentDocument::CommentDocument(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json CommentDocument::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор комментария
    if (m_commentId.has_value())
    {
        result["comment_id"] = m_commentId.value();
    }
    // Идентификатор документа
    if (m_documentId.has_value())
    {
        result["document_id"] = m_documentId.value();
    }

    return result;
}

bool CommentDocument::fromJson(const nlohmann::json& json)
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
    // Идентификатор комментария
    if (json.contains("comment_id") && !json["comment_id"].is_null())
    {
        try
        {
            m_commentId = json["comment_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_commentId = std::nullopt;
    }
    // Идентификатор документа
    if (json.contains("document_id") && !json["document_id"].is_null())
    {
        try
        {
            m_documentId = json["document_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_documentId = std::nullopt;
    }

    return success;
}

bool CommentDocument::isValid() const
{
    if (!m_commentId.has_value())
    {
        return false;
    }
    if (!m_documentId.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string CommentDocument::validationError() const
{
    if (!m_commentId.has_value())
    {
        return "Поле «comment_id» является обязательным для заполнения";
    }
    if (!m_documentId.has_value())
    {
        return "Поле «document_id» является обязательным для заполнения";
    }


    return "";
}

bool CommentDocument::operator==(const CommentDocument& other) const
{
    return
        m_id == other.m_id
        && m_commentId == other.m_commentId
        && m_documentId == other.m_documentId
;
}

} // namespace dto