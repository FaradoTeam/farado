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
 * @brief Comment DTO
 */
class Comment final
{
public:
    using Ptr = std::shared_ptr<Comment>;
    using ConstPtr = std::shared_ptr<const Comment>;

public:
    Comment() = default;
    explicit Comment(const nlohmann::json& json);

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

    // Поле: «userId» Идентификатор автора
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

    // Поле: «createdAt» Время создания
    const std::optional<std::chrono::system_clock::time_point>& createdAt() const { return m_createdAt; }
    bool hasCreatedAt() const { return m_createdAt.has_value(); }
    void setCreatedAt(const std::chrono::system_clock::time_point& value)
    {
        m_createdAt = value;
    }
    void setCreatedAt(std::chrono::system_clock::time_point&& value)
    {
        m_createdAt = std::move(value);
    }
    void clearCreatedAt()
    {
        m_createdAt = std::nullopt;
    }

    // Поле: «content» Текст комментария
    const std::optional<std::string>& content() const { return m_content; }
    bool hasContent() const { return m_content.has_value(); }
    void setContent(const std::string& value)
    {
        m_content = value;
    }
    void setContent(std::string&& value)
    {
        m_content = std::move(value);
    }
    void clearContent()
    {
        m_content = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Comment& other) const;
    bool operator!=(const Comment& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Comment& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_userId;
    std::optional<int64_t> m_itemId;
    std::optional<std::chrono::system_clock::time_point> m_createdAt;
    std::optional<std::string> m_content;
};

inline std::ostream& operator<<(std::ostream& os, const Comment& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto