#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item_type.h"

namespace dto
{

ItemType::ItemType(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json ItemType::toJson() const
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
    // Состояние по умолчанию для новых элементов
    if (m_defaultStateId.has_value())
    {
        result["default_state_id"] = m_defaultStateId.value();
    }
    // Название типа
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Вид элемента
    if (m_kind.has_value())
    {
        result["kind"] = m_kind.value();
    }
    // Содержимое по умолчанию (шаблон для создания новых item)
    if (m_defaultContent.has_value())
    {
        result["default_content"] = m_defaultContent.value();
    }

    return result;
}

bool ItemType::fromJson(const nlohmann::json& json)
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
    // Состояние по умолчанию для новых элементов
    if (json.contains("default_state_id") && !json["default_state_id"].is_null())
    {
        try
        {
            m_defaultStateId = json["default_state_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_defaultStateId = std::nullopt;
    }
    // Название типа
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
    // Вид элемента
    if (json.contains("kind") && !json["kind"].is_null())
    {
        try
        {
            m_kind = json["kind"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_kind = std::nullopt;
    }
    // Содержимое по умолчанию (шаблон для создания новых item)
    if (json.contains("default_content") && !json["default_content"].is_null())
    {
        try
        {
            m_defaultContent = json["default_content"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_defaultContent = std::nullopt;
    }

    return success;
}

bool ItemType::isValid() const
{
    if (!m_workflowId.has_value())
    {
        return false;
    }
    if (!m_defaultStateId.has_value())
    {
        return false;
    }
    if (!m_caption.has_value())
    {
        return false;
    }
    if (!m_kind.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений
    if (m_caption.value().empty())
    {
        return false;
    }
    if (m_kind.value().empty())
    {
        return false;
    }

    return true;
}

std::string ItemType::validationError() const
{
    if (!m_workflowId.has_value())
    {
        return "Поле «workflow_id» является обязательным для заполнения";
    }
    if (!m_defaultStateId.has_value())
    {
        return "Поле «default_state_id» является обязательным для заполнения";
    }
    if (!m_caption.has_value())
    {
        return "Поле «caption» является обязательным для заполнения";
    }
    if (!m_kind.has_value())
    {
        return "Поле «kind» является обязательным для заполнения";
    }

    if (m_caption.value().empty())
    {
        return "Поле «caption» не может быть пустой строкой";
    }
    if (m_kind.value().empty())
    {
        return "Поле «kind» не может быть пустой строкой";
    }

    return "";
}

bool ItemType::operator==(const ItemType& other) const
{
    return
        m_id == other.m_id
        && m_workflowId == other.m_workflowId
        && m_defaultStateId == other.m_defaultStateId
        && m_caption == other.m_caption
        && m_kind == other.m_kind
        && m_defaultContent == other.m_defaultContent
;
}

} // namespace dto