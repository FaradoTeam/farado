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
 * @brief RuleState DTO
 */
class RuleState final
{
public:
    using Ptr = std::shared_ptr<RuleState>;
    using ConstPtr = std::shared_ptr<const RuleState>;

public:
    RuleState() = default;
    explicit RuleState(const nlohmann::json& json);

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

    // Поле: «ruleId» Идентификатор правила
    const std::optional<int64_t>& ruleId() const { return m_ruleId; }
    bool hasRuleId() const { return m_ruleId.has_value(); }
    void setRuleId(const int64_t& value)
    {
        m_ruleId = value;
    }
    void setRuleId(int64_t&& value)
    {
        m_ruleId = std::move(value);
    }
    void clearRuleId()
    {
        m_ruleId = std::nullopt;
    }

    // Поле: «stateId» Идентификатор целевого состояния
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

    // Поле: «isStateAllowed» Разрешение на перевод элемента в это состояние
    const std::optional<bool>& isStateAllowed() const { return m_isStateAllowed; }
    bool hasIsStateAllowed() const { return m_isStateAllowed.has_value(); }
    void setIsStateAllowed(const bool& value)
    {
        m_isStateAllowed = value;
    }
    void setIsStateAllowed(bool&& value)
    {
        m_isStateAllowed = std::move(value);
    }
    void clearIsStateAllowed()
    {
        m_isStateAllowed = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const RuleState& other) const;
    bool operator!=(const RuleState& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const RuleState& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_ruleId;
    std::optional<int64_t> m_stateId;
    std::optional<bool> m_isStateAllowed;
};

inline std::ostream& operator<<(std::ostream& os, const RuleState& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto