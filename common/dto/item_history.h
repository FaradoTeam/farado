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
 * @brief ItemHistory DTO
 */
class ItemHistory final
{
public:
    using Ptr = std::shared_ptr<ItemHistory>;
    using ConstPtr = std::shared_ptr<const ItemHistory>;

public:
    ItemHistory() = default;
    explicit ItemHistory(const nlohmann::json& json);

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

    // Поле: «timestamp» Время изменения
    const std::optional<std::chrono::system_clock::time_point>& timestamp() const { return m_timestamp; }
    bool hasTimestamp() const { return m_timestamp.has_value(); }
    void setTimestamp(const std::chrono::system_clock::time_point& value)
    {
        m_timestamp = value;
    }
    void setTimestamp(std::chrono::system_clock::time_point&& value)
    {
        m_timestamp = std::move(value);
    }
    void clearTimestamp()
    {
        m_timestamp = std::nullopt;
    }

    // Поле: «diff» JSON с изменениями (diff)
    const std::optional<std::string>& diff() const { return m_diff; }
    bool hasDiff() const { return m_diff.has_value(); }
    void setDiff(const std::string& value)
    {
        m_diff = value;
    }
    void setDiff(std::string&& value)
    {
        m_diff = std::move(value);
    }
    void clearDiff()
    {
        m_diff = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ItemHistory& other) const;
    bool operator!=(const ItemHistory& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ItemHistory& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemId;
    std::optional<int64_t> m_userId;
    std::optional<std::chrono::system_clock::time_point> m_timestamp;
    std::optional<std::string> m_diff;
};

inline std::ostream& operator<<(std::ostream& os, const ItemHistory& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto