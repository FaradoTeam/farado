#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "board_column.h"

namespace dto
{

BoardColumn::BoardColumn(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json BoardColumn::toJson() const
{
    nlohmann::json result;

    // Уникальный идентификатор
    if (m_id.has_value())
    {
        result["id"] = m_id.value();
    }
    // Идентификатор доски
    if (m_boardId.has_value())
    {
        result["board_id"] = m_boardId.value();
    }
    // Идентификатор состояния
    if (m_stateId.has_value())
    {
        result["state_id"] = m_stateId.value();
    }
    // Порядковый номер колонки на доске
    if (m_orderNumber.has_value())
    {
        result["order_number"] = m_orderNumber.value();
    }
    // JSON с настройками (WIP limit
    if (m_settings.has_value())
    {
        result["settings"] = m_settings.value();
    }

    return result;
}

bool BoardColumn::fromJson(const nlohmann::json& json)
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
    // Идентификатор доски
    if (json.contains("board_id") && !json["board_id"].is_null())
    {
        try
        {
            m_boardId = json["board_id"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_boardId = std::nullopt;
    }
    // Идентификатор состояния
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
    // Порядковый номер колонки на доске
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
    // JSON с настройками (WIP limit
    if (json.contains("settings") && !json["settings"].is_null())
    {
        try
        {
            m_settings = json["settings"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_settings = std::nullopt;
    }

    return success;
}

bool BoardColumn::isValid() const
{
    if (!m_boardId.has_value())
    {
        return false;
    }
    if (!m_stateId.has_value())
    {
        return false;
    }
    if (!m_orderNumber.has_value())
    {
        return false;
    }

    // Дополнительные проверки для непустых значений

    return true;
}

std::string BoardColumn::validationError() const
{
    if (!m_boardId.has_value())
    {
        return "Поле «board_id» является обязательным для заполнения";
    }
    if (!m_stateId.has_value())
    {
        return "Поле «state_id» является обязательным для заполнения";
    }
    if (!m_orderNumber.has_value())
    {
        return "Поле «order_number» является обязательным для заполнения";
    }


    return "";
}

bool BoardColumn::operator==(const BoardColumn& other) const
{
    return
        m_id == other.m_id
        && m_boardId == other.m_boardId
        && m_stateId == other.m_stateId
        && m_orderNumber == other.m_orderNumber
        && m_settings == other.m_settings
;
}

} // namespace dto