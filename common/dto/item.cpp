#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "item.h"

namespace dto
{

Item::Item(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json Item::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор типа элемента
    if (m_itemTypeId.has_value())
    {
        result["item_type_id"] = m_itemTypeId.value();
    }
    // Идентификатор родительского элемента
    if (m_parentId.has_value())
    {
        result["parent_id"] = m_parentId.value();
    }
    // Идентификатор текущего состояния
    if (m_stateId.has_value())
    {
        result["state_id"] = m_stateId.value();
    }
    // Идентификатор фазы
    if (m_phaseId.has_value())
    {
        result["phase_id"] = m_phaseId.value();
    }
    // Название элемента
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Содержимое элемента
    if (m_content.has_value())
    {
        result["content"] = m_content.value();
    }
    // Флаг удаления (мягкое удаление)
    if (m_isDeleted.has_value())
    {
        result["is_deleted"] = m_isDeleted.value();
    }

    return result;
}

bool Item::fromJson(const nlohmann::json& json)
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
    // Идентификатор родительского элемента
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
    // Идентификатор текущего состояния
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
    // Название элемента
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
    // Содержимое элемента
    if (json.contains("content") && !json["content"].is_null())
    {
        try
        {
            m_content = json["content"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_content = std::nullopt;
    }
    // Флаг удаления (мягкое удаление)
    if (json.contains("is_deleted") && !json["is_deleted"].is_null())
    {
        try
        {
            m_isDeleted = json["is_deleted"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isDeleted = std::nullopt;
    }

    return success;
}

bool Item::isValid() const
{
    if (!m_itemTypeId.has_value())
    {
        return false;
    }
    if (!m_stateId.has_value())
    {
        return false;
    }
    if (!m_phaseId.has_value())
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

std::string Item::validationError() const
{
    if (!m_itemTypeId.has_value())
    {
        return "Поле «item_type_id» является обязательным для заполнения";
    }
    if (!m_stateId.has_value())
    {
        return "Поле «state_id» является обязательным для заполнения";
    }
    if (!m_phaseId.has_value())
    {
        return "Поле «phase_id» является обязательным для заполнения";
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

bool Item::operator==(const Item& other) const
{
    return
        m_id == other.m_id
        && m_itemTypeId == other.m_itemTypeId
        && m_parentId == other.m_parentId
        && m_stateId == other.m_stateId
        && m_phaseId == other.m_phaseId
        && m_caption == other.m_caption
        && m_content == other.m_content
        && m_isDeleted == other.m_isDeleted
;
}

} // namespace dto