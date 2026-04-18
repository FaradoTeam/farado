#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "state.h"

namespace dto
{

State::State(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json State::toJson() const
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
    // Название состояния
    if (m_caption.has_value())
    {
        result["caption"] = m_caption.value();
    }
    // Описание состояния
    if (m_description.has_value())
    {
        result["description"] = m_description.value();
    }
    // Вес состояния (для метрик)
    if (m_weight.has_value())
    {
        result["weight"] = m_weight.value();
    }
    // Порядковый номер для сортировки
    if (m_orderNumber.has_value())
    {
        result["order_number"] = m_orderNumber.value();
    }
    // Флаг архивного состояния (редко отображается)
    if (m_isArchive.has_value())
    {
        result["is_archive"] = m_isArchive.value();
    }
    // Флаг очереди (для определения активных работ)
    if (m_isQueue.has_value())
    {
        result["is_queue"] = m_isQueue.value();
    }

    return result;
}

bool State::fromJson(const nlohmann::json& json)
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
    // Название состояния
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
    // Описание состояния
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
    // Вес состояния (для метрик)
    if (json.contains("weight") && !json["weight"].is_null())
    {
        try
        {
            m_weight = json["weight"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_weight = std::nullopt;
    }
    // Порядковый номер для сортировки
    if (json.contains("order_number") && !json["order_number"].is_null())
    {
        try
        {
            m_orderNumber = json["order_number"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_orderNumber = std::nullopt;
    }
    // Флаг архивного состояния (редко отображается)
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
    // Флаг очереди (для определения активных работ)
    if (json.contains("is_queue") && !json["is_queue"].is_null())
    {
        try
        {
            m_isQueue = json["is_queue"].get<bool>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_isQueue = std::nullopt;
    }

    return success;
}

bool State::isValid() const
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

std::string State::validationError() const
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

bool State::operator==(const State& other) const
{
    return
        m_id == other.m_id
        && m_workflowId == other.m_workflowId
        && m_caption == other.m_caption
        && m_description == other.m_description
        && m_weight == other.m_weight
        && m_orderNumber == other.m_orderNumber
        && m_isArchive == other.m_isArchive
        && m_isQueue == other.m_isQueue
;
}

} // namespace dto