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
 * @brief UserAction DTO
 */
class UserAction final
{
public:
    using Ptr = std::shared_ptr<UserAction>;
    using ConstPtr = std::shared_ptr<const UserAction>;

public:
    UserAction() = default;
    explicit UserAction(const nlohmann::json& json);

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

    // Поле: «timestamp» Время действия
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

    // Поле: «caption» Краткое описание действия
    const std::optional<std::string>& caption() const { return m_caption; }
    bool hasCaption() const { return m_caption.has_value(); }
    void setCaption(const std::string& value)
    {
        m_caption = value;
    }
    void setCaption(std::string&& value)
    {
        m_caption = std::move(value);
    }
    void clearCaption()
    {
        m_caption = std::nullopt;
    }

    // Поле: «description» Подробное описание (контекст)
    const std::optional<std::string>& description() const { return m_description; }
    bool hasDescription() const { return m_description.has_value(); }
    void setDescription(const std::string& value)
    {
        m_description = value;
    }
    void setDescription(std::string&& value)
    {
        m_description = std::move(value);
    }
    void clearDescription()
    {
        m_description = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const UserAction& other) const;
    bool operator!=(const UserAction& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const UserAction& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_userId;
    std::optional<std::chrono::system_clock::time_point> m_timestamp;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
};

inline std::ostream& operator<<(std::ostream& os, const UserAction& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto