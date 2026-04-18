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
 * @brief Project DTO
 */
class Project final
{
public:
    using Ptr = std::shared_ptr<Project>;
    using ConstPtr = std::shared_ptr<const Project>;

public:
    Project() = default;
    explicit Project(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор проекта
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

    // Поле: «parentId» Идентификатор родительского проекта (null для корневых)
    const std::optional<int64_t>& parentId() const { return m_parentId; }
    bool hasParentId() const { return m_parentId.has_value(); }
    void setParentId(const int64_t& value)
    {
        m_parentId = value;
    }
    void setParentId(int64_t&& value)
    {
        m_parentId = std::move(value);
    }
    void clearParentId()
    {
        m_parentId = std::nullopt;
    }

    // Поле: «caption» Название проекта
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

    // Поле: «description» Описание проекта
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

    // Поле: «createdAt» Дата и время создания
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

    // Поле: «updatedAt» Дата и время последнего обновления
    const std::optional<std::chrono::system_clock::time_point>& updatedAt() const { return m_updatedAt; }
    bool hasUpdatedAt() const { return m_updatedAt.has_value(); }
    void setUpdatedAt(const std::chrono::system_clock::time_point& value)
    {
        m_updatedAt = value;
    }
    void setUpdatedAt(std::chrono::system_clock::time_point&& value)
    {
        m_updatedAt = std::move(value);
    }
    void clearUpdatedAt()
    {
        m_updatedAt = std::nullopt;
    }

    // Поле: «isArchive» Флаг архивации (мягкое удаление)
    const std::optional<bool>& isArchive() const { return m_isArchive; }
    bool hasIsArchive() const { return m_isArchive.has_value(); }
    void setIsArchive(const bool& value)
    {
        m_isArchive = value;
    }
    void setIsArchive(bool&& value)
    {
        m_isArchive = std::move(value);
    }
    void clearIsArchive()
    {
        m_isArchive = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Project& other) const;
    bool operator!=(const Project& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Project& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_parentId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
    std::optional<std::chrono::system_clock::time_point> m_createdAt;
    std::optional<std::chrono::system_clock::time_point> m_updatedAt;
    std::optional<bool> m_isArchive;
};

inline std::ostream& operator<<(std::ostream& os, const Project& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto