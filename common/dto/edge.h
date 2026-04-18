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
 * @brief Edge DTO
 */
class Edge final
{
public:
    using Ptr = std::shared_ptr<Edge>;
    using ConstPtr = std::shared_ptr<const Edge>;

public:
    Edge() = default;
    explicit Edge(const nlohmann::json& json);

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

    // Поле: «beginStateId» Начальное состояние
    const std::optional<int64_t>& beginStateId() const { return m_beginStateId; }
    bool hasBeginStateId() const { return m_beginStateId.has_value(); }
    void setBeginStateId(const int64_t& value)
    {
        m_beginStateId = value;
    }
    void setBeginStateId(int64_t&& value)
    {
        m_beginStateId = std::move(value);
    }
    void clearBeginStateId()
    {
        m_beginStateId = std::nullopt;
    }

    // Поле: «endStateId» Конечное состояние
    const std::optional<int64_t>& endStateId() const { return m_endStateId; }
    bool hasEndStateId() const { return m_endStateId.has_value(); }
    void setEndStateId(const int64_t& value)
    {
        m_endStateId = value;
    }
    void setEndStateId(int64_t&& value)
    {
        m_endStateId = std::move(value);
    }
    void clearEndStateId()
    {
        m_endStateId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Edge& other) const;
    bool operator!=(const Edge& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Edge& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_beginStateId;
    std::optional<int64_t> m_endStateId;
};

inline std::ostream& operator<<(std::ostream& os, const Edge& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto