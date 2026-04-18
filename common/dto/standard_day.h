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
 * @brief StandardDay DTO
 */
class StandardDay final
{
public:
    using Ptr = std::shared_ptr<StandardDay>;
    using ConstPtr = std::shared_ptr<const StandardDay>;

public:
    StandardDay() = default;
    explicit StandardDay(const nlohmann::json& json);

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

    // Поле: «weekDayNumber» Номер дня недели (0-6)
    const std::optional<int64_t>& weekDayNumber() const { return m_weekDayNumber; }
    bool hasWeekDayNumber() const { return m_weekDayNumber.has_value(); }
    void setWeekDayNumber(const int64_t& value)
    {
        m_weekDayNumber = value;
    }
    void setWeekDayNumber(int64_t&& value)
    {
        m_weekDayNumber = std::move(value);
    }
    void clearWeekDayNumber()
    {
        m_weekDayNumber = std::nullopt;
    }

    // Поле: «weekOrder» Порядковый номер недели (для сменного графика)
    const std::optional<int64_t>& weekOrder() const { return m_weekOrder; }
    bool hasWeekOrder() const { return m_weekOrder.has_value(); }
    void setWeekOrder(const int64_t& value)
    {
        m_weekOrder = value;
    }
    void setWeekOrder(int64_t&& value)
    {
        m_weekOrder = std::move(value);
    }
    void clearWeekOrder()
    {
        m_weekOrder = std::nullopt;
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

    // Поле: «beginWorkTime» Время начала работы (HH:MM)
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

    // Поле: «endWorkTime» Время окончания работы (HH:MM)
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
    bool operator==(const StandardDay& other) const;
    bool operator!=(const StandardDay& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const StandardDay& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_weekDayNumber;
    std::optional<int64_t> m_weekOrder;
    std::optional<bool> m_isWorkDay;
    std::optional<std::string> m_beginWorkTime;
    std::optional<std::string> m_endWorkTime;
    std::optional<int64_t> m_breakDuration;
};

inline std::ostream& operator<<(std::ostream& os, const StandardDay& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto