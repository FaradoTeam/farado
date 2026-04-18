#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_document.h"

namespace dto
{

ItemDocument::ItemDocument(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemDocument::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор элемента
    if (m_itemId.has_value())
    {
        result["item_id"] = m_itemId.value();
    }
    // Идентификатор документа
    if (m_documentId.has_value())
    {
        result["document_id"] = m_documentId.value();
    }

    return result;
}

bool ItemDocument::fromJson(const nlohmann::json& json)
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
    // Идентификатор элемента
    if (json.contains("item_id") && !json["item_id"].is_null())
    {
        try
        {
            m_itemId = json["item_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_itemId = std::nullopt;
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

bool ItemDocument::isValid() const
{
    if (!m_itemId.has_value())
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

std::string ItemDocument::validationError() const
{
    if (!m_itemId.has_value())
    {
        return "Поле «item_id» является обязательным для заполнения";
    }
    if (!m_documentId.has_value())
    {
        return "Поле «document_id» является обязательным для заполнения";
    }


    return "";
}

bool ItemDocument::operator==(const ItemDocument& other) const
{
    return
        m_id == other.m_id
        && m_itemId == other.m_itemId
        && m_documentId == other.m_documentId
;
}

} // namespace dto