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
 * @brief Rule DTO
 */
class Rule final
{
public:
    using Ptr = std::shared_ptr<Rule>;
    using ConstPtr = std::shared_ptr<const Rule>;

public:
    Rule() = default;
    explicit Rule(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор правила
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

    // Поле: «roleId» Идентификатор роли
    const std::optional<int64_t>& roleId() const { return m_roleId; }
    bool hasRoleId() const { return m_roleId.has_value(); }
    void setRoleId(const int64_t& value)
    {
        m_roleId = value;
    }
    void setRoleId(int64_t&& value)
    {
        m_roleId = std::move(value);
    }
    void clearRoleId()
    {
        m_roleId = std::nullopt;
    }

    // Поле: «isRootProjectCreator» Право на создание корневых проектов
    const std::optional<bool>& isRootProjectCreator() const { return m_isRootProjectCreator; }
    bool hasIsRootProjectCreator() const { return m_isRootProjectCreator.has_value(); }
    void setIsRootProjectCreator(const bool& value)
    {
        m_isRootProjectCreator = value;
    }
    void setIsRootProjectCreator(bool&& value)
    {
        m_isRootProjectCreator = std::move(value);
    }
    void clearIsRootProjectCreator()
    {
        m_isRootProjectCreator = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Rule& other) const;
    bool operator!=(const Rule& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Rule& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_roleId;
    std::optional<bool> m_isRootProjectCreator;
};

inline std::ostream& operator<<(std::ostream& os, const Rule& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto