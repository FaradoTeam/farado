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
 * @brief Phase DTO
 */
class Phase final
{
public:
    using Ptr = std::shared_ptr<Phase>;
    using ConstPtr = std::shared_ptr<const Phase>;

public:
    Phase() = default;
    explicit Phase(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор фазы
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

    // Поле: «projectId» Идентификатор проекта
    const std::optional<int64_t>& projectId() const { return m_projectId; }
    bool hasProjectId() const { return m_projectId.has_value(); }
    void setProjectId(const int64_t& value)
    {
        m_projectId = value;
    }
    void setProjectId(int64_t&& value)
    {
        m_projectId = std::move(value);
    }
    void clearProjectId()
    {
        m_projectId = std::nullopt;
    }

    // Поле: «caption» Название фазы
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

    // Поле: «description» Описание фазы
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

    // Поле: «beginDate» Дата начала фазы
    const std::optional<std::chrono::system_clock::time_point>& beginDate() const { return m_beginDate; }
    bool hasBeginDate() const { return m_beginDate.has_value(); }
    void setBeginDate(const std::chrono::system_clock::time_point& value)
    {
        m_beginDate = value;
    }
    void setBeginDate(std::chrono::system_clock::time_point&& value)
    {
        m_beginDate = std::move(value);
    }
    void clearBeginDate()
    {
        m_beginDate = std::nullopt;
    }

    // Поле: «endDate» Дата окончания фазы
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

    // Поле: «isArchive» Флаг архивации фазы
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
    bool operator==(const Phase& other) const;
    bool operator!=(const Phase& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Phase& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_projectId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
    std::optional<std::chrono::system_clock::time_point> m_beginDate;
    std::optional<std::chrono::system_clock::time_point> m_endDate;
    std::optional<bool> m_isArchive;
};

inline std::ostream& operator<<(std::ostream& os, const Phase& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto