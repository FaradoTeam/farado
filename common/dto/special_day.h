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
 * @brief SpecialDay DTO
 */
class SpecialDay final
{
public:
    using Ptr = std::shared_ptr<SpecialDay>;
    using ConstPtr = std::shared_ptr<const SpecialDay>;

public:
    SpecialDay() = default;
    explicit SpecialDay(const nlohmann::json& json);

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

    // Поле: «date» Дата (уникальная)
    const std::optional<std::chrono::system_clock::time_point>& date() const { return m_date; }
    bool hasDate() const { return m_date.has_value(); }
    void setDate(const std::chrono::system_clock::time_point& value)
    {
        m_date = value;
    }
    void setDate(std::chrono::system_clock::time_point&& value)
    {
        m_date = std::move(value);
    }
    void clearDate()
    {
        m_date = std::nullopt;
    }

    // Поле: «isWorkDay» Является ли день рабочим
    const std::optional<bool>& isWorkDay() const { return m_isWorkDay; }
    bool hasIsWorkDay() const { return m_isWorkDay.has_value(); }
    void setIsWorkDay(const bool& value)
    {
        m_isWorkDay = value;
    }
    void setIsWorkDay(bool&& value)
    {
        m_isWorkDay = std::move(value);
    }
    void clearIsWorkDay()
    {
        m_isWorkDay = std::nullopt;
    }

    // Поле: «beginWorkTime» Время начала работы (если рабочий)
    const std::optional<std::string>& beginWorkTime() const { return m_beginWorkTime; }
    bool hasBeginWorkTime() const { return m_beginWorkTime.has_value(); }
    void setBeginWorkTime(const std::string& value)
    {
        m_beginWorkTime = value;
    }
    void setBeginWorkTime(std::string&& value)
    {
        m_beginWorkTime = std::move(value);
    }
    void clearBeginWorkTime()
    {
        m_beginWorkTime = std::nullopt;
    }

    // Поле: «endWorkTime» Время окончания работы (если рабочий)
    const std::optional<std::string>& endWorkTime() const { return m_endWorkTime; }
    bool hasEndWorkTime() const { return m_endWorkTime.has_value(); }
    void setEndWorkTime(const std::string& value)
    {
        m_endWorkTime = value;
    }
    void setEndWorkTime(std::string&& value)
    {
        m_endWorkTime = std::move(value);
    }
    void clearEndWorkTime()
    {
        m_endWorkTime = std::nullopt;
    }

    // Поле: «breakDuration» Длительность перерыва в минутах
    const std::optional<int64_t>& breakDuration() const { return m_breakDuration; }
    bool hasBreakDuration() const { return m_breakDuration.has_value(); }
    void setBreakDuration(const int64_t& value)
    {
        m_breakDuration = value;
    }
    void setBreakDuration(int64_t&& value)
    {
        m_breakDuration = std::move(value);
    }
    void clearBreakDuration()
    {
        m_breakDuration = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const SpecialDay& other) const;
    bool operator!=(const SpecialDay& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const SpecialDay& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<std::chrono::system_clock::time_point> m_date;
    std::optional<bool> m_isWorkDay;
    std::optional<std::string> m_beginWorkTime;
    std::optional<std::string> m_endWorkTime;
    std::optional<int64_t> m_breakDuration;
};

inline std::ostream& operator<<(std::ostream& os, const SpecialDay& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto