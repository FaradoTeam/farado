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
 * @brief TeamMessage DTO
 */
class TeamMessage final
{
public:
    using Ptr = std::shared_ptr<TeamMessage>;
    using ConstPtr = std::shared_ptr<const TeamMessage>;

public:
    TeamMessage() = default;
    explicit TeamMessage(const nlohmann::json& json);

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

    // Поле: «senderUserId» Идентификатор отправителя
    const std::optional<int64_t>& senderUserId() const { return m_senderUserId; }
    bool hasSenderUserId() const { return m_senderUserId.has_value(); }
    void setSenderUserId(const int64_t& value)
    {
        m_senderUserId = value;
    }
    void setSenderUserId(int64_t&& value)
    {
        m_senderUserId = std::move(value);
    }
    void clearSenderUserId()
    {
        m_senderUserId = std::nullopt;
    }

    // Поле: «teamId» Идентификатор команды
    const std::optional<int64_t>& teamId() const { return m_teamId; }
    bool hasTeamId() const { return m_teamId.has_value(); }
    void setTeamId(const int64_t& value)
    {
        m_teamId = value;
    }
    void setTeamId(int64_t&& value)
    {
        m_teamId = std::move(value);
    }
    void clearTeamId()
    {
        m_teamId = std::nullopt;
    }

    // Поле: «creationTimestamp» Время отправки
    const std::optional<std::chrono::system_clock::time_point>& creationTimestamp() const { return m_creationTimestamp; }
    bool hasCreationTimestamp() const { return m_creationTimestamp.has_value(); }
    void setCreationTimestamp(const std::chrono::system_clock::time_point& value)
    {
        m_creationTimestamp = value;
    }
    void setCreationTimestamp(std::chrono::system_clock::time_point&& value)
    {
        m_creationTimestamp = std::move(value);
    }
    void clearCreationTimestamp()
    {
        m_creationTimestamp = std::nullopt;
    }

    // Поле: «content» Текст сообщения
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
    bool operator==(const TeamMessage& other) const;
    bool operator!=(const TeamMessage& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const TeamMessage& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_senderUserId;
    std::optional<int64_t> m_teamId;
    std::optional<std::chrono::system_clock::time_point> m_creationTimestamp;
    std::optional<std::string> m_content;
};

inline std::ostream& operator<<(std::ostream& os, const TeamMessage& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto