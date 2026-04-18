#pragma once

#include <optional>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "common/helpers/print_helpers.h"


namespace dto
{

/**
 * @brief BoardColumn DTO
 */
class BoardColumn final
{
public:
    using Ptr = std::shared_ptr<BoardColumn>;
    using ConstPtr = std::shared_ptr<const BoardColumn>;

public:
    BoardColumn() = default;
    explicit BoardColumn(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор
    const std::optional<int64_t>& id() const { return m_id; }
    bool hasId() const { return m_id.has_value(); }
    void setId(const int64_t& value)
    {
        m_id = value;
    }
    void setId(int64_t&& value)
    {
        m_id = std::move(value);
    }
    void clearId()
    {
        m_id = std::nullopt;
    }

    // Поле: «boardId» Идентификатор доски
    const std::optional<int64_t>& boardId() const { return m_boardId; }
    bool hasBoardId() const { return m_boardId.has_value(); }
    void setBoardId(const int64_t& value)
    {
        m_boardId = value;
    }
    void setBoardId(int64_t&& value)
    {
        m_boardId = std::move(value);
    }
    void clearBoardId()
    {
        m_boardId = std::nullopt;
    }

    // Поле: «stateId» Идентификатор состояния
    const std::optional<int64_t>& stateId() const { return m_stateId; }
    bool hasStateId() const { return m_stateId.has_value(); }
    void setStateId(const int64_t& value)
    {
        m_stateId = value;
    }
    void setStateId(int64_t&& value)
    {
        m_stateId = std::move(value);
    }
    void clearStateId()
    {
        m_stateId = std::nullopt;
    }

    // Поле: «orderNumber» Порядковый номер колонки на доске
    const std::optional<int64_t>& orderNumber() const { return m_orderNumber; }
    bool hasOrderNumber() const { return m_orderNumber.has_value(); }
    void setOrderNumber(const int64_t& value)
    {
        m_orderNumber = value;
    }
    void setOrderNumber(int64_t&& value)
    {
        m_orderNumber = std::move(value);
    }
    void clearOrderNumber()
    {
        m_orderNumber = std::nullopt;
    }

    // Поле: «settings» JSON с настройками (WIP limit
    const std::optional<std::string>& settings() const { return m_settings; }
    bool hasSettings() const { return m_settings.has_value(); }
    void setSettings(const std::string& value)
    {
        m_settings = value;
    }
    void setSettings(std::string&& value)
    {
        m_settings = std::move(value);
    }
    void clearSettings()
    {
        m_settings = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const BoardColumn& other) const;
    bool operator!=(const BoardColumn& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const BoardColumn& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_boardId;
    std::optional<int64_t> m_stateId;
    std::optional<int64_t> m_orderNumber;
    std::optional<std::string> m_settings;
};

inline std::ostream& operator<<(std::ostream& os, const BoardColumn& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto