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
 * @brief UserTodo DTO
 */
class UserTodo final
{
public:
    using Ptr = std::shared_ptr<UserTodo>;
    using ConstPtr = std::shared_ptr<const UserTodo>;

public:
    UserTodo() = default;
    explicit UserTodo(const nlohmann::json& json);

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

    // Поле: «itemId» Идентификатор элемента
    const std::optional<int64_t>& itemId() const { return m_itemId; }
    bool hasItemId() const { return m_itemId.has_value(); }
    void setItemId(const int64_t& value)
    {
        m_itemId = value;
    }
    void setItemId(int64_t&& value)
    {
        m_itemId = std::move(value);
    }
    void clearItemId()
    {
        m_itemId = std::nullopt;
    }

    // Поле: «userId» Идентификатор пользователя
    const std::optional<int64_t>& userId() const { return m_userId; }
    bool hasUserId() const { return m_userId.has_value(); }
    void setUserId(const int64_t& value)
    {
        m_userId = value;
    }
    void setUserId(int64_t&& value)
    {
        m_userId = std::move(value);
    }
    void clearUserId()
    {
        m_userId = std::nullopt;
    }

    // Поле: «startDate» Плановая дата начала
    const std::optional<std::chrono::system_clock::time_point>& startDate() const { return m_startDate; }
    bool hasStartDate() const { return m_startDate.has_value(); }
    void setStartDate(const std::chrono::system_clock::time_point& value)
    {
        m_startDate = value;
    }
    void setStartDate(std::chrono::system_clock::time_point&& value)
    {
        m_startDate = std::move(value);
    }
    void clearStartDate()
    {
        m_startDate = std::nullopt;
    }

    // Поле: «endDate» Плановая дата окончания
    const std::optional<std::chrono::system_clock::time_point>& endDate() const { return m_endDate; }
    bool hasEndDate() const { return m_endDate.has_value(); }
    void setEndDate(const std::chrono::system_clock::time_point& value)
    {
        m_endDate = value;
    }
    void setEndDate(std::chrono::system_clock::time_point&& value)
    {
        m_endDate = std::move(value);
    }
    void clearEndDate()
    {
        m_endDate = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const UserTodo& other) const;
    bool operator!=(const UserTodo& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const UserTodo& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemId;
    std::optional<int64_t> m_userId;
    std::optional<std::chrono::system_clock::time_point> m_startDate;
    std::optional<std::chrono::system_clock::time_point> m_endDate;
};

inline std::ostream& operator<<(std::ostream& os, const UserTodo& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto