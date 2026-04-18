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
 * @brief RuleItemType DTO
 */
class RuleItemType final
{
public:
    using Ptr = std::shared_ptr<RuleItemType>;
    using ConstPtr = std::shared_ptr<const RuleItemType>;

public:
    RuleItemType() = default;
    explicit RuleItemType(const nlohmann::json& json);

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

    // Поле: «itemTypeId» Идентификатор типа элемента
    const std::optional<int64_t>& itemTypeId() const { return m_itemTypeId; }
    bool hasItemTypeId() const { return m_itemTypeId.has_value(); }
    void setItemTypeId(const int64_t& value)
    {
        m_itemTypeId = value;
    }
    void setItemTypeId(int64_t&& value)
    {
        m_itemTypeId = std::move(value);
    }
    void clearItemTypeId()
    {
        m_itemTypeId = std::nullopt;
    }

    // Поле: «isReader» Право на чтение элементов данного типа
    const std::optional<bool>& isReader() const { return m_isReader; }
    bool hasIsReader() const { return m_isReader.has_value(); }
    void setIsReader(const bool& value)
    {
        m_isReader = value;
    }
    void setIsReader(bool&& value)
    {
        m_isReader = std::move(value);
    }
    void clearIsReader()
    {
        m_isReader = std::nullopt;
    }

    // Поле: «isWriter» Право на создание и изменение элементов данного типа
    const std::optional<bool>& isWriter() const { return m_isWriter; }
    bool hasIsWriter() const { return m_isWriter.has_value(); }
    void setIsWriter(const bool& value)
    {
        m_isWriter = value;
    }
    void setIsWriter(bool&& value)
    {
        m_isWriter = std::move(value);
    }
    void clearIsWriter()
    {
        m_isWriter = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const RuleItemType& other) const;
    bool operator!=(const RuleItemType& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const RuleItemType& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_ruleId;
    std::optional<int64_t> m_itemTypeId;
    std::optional<bool> m_isReader;
    std::optional<bool> m_isWriter;
};

inline std::ostream& operator<<(std::ostream& os, const RuleItemType& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto