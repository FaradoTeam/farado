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
 * @brief ItemLink DTO
 */
class ItemLink final
{
public:
    using Ptr = std::shared_ptr<ItemLink>;
    using ConstPtr = std::shared_ptr<const ItemLink>;

public:
    ItemLink() = default;
    explicit ItemLink(const nlohmann::json& json);

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

    // Поле: «linkTypeId» Идентификатор типа связи
    const std::optional<int64_t>& linkTypeId() const { return m_linkTypeId; }
    bool hasLinkTypeId() const { return m_linkTypeId.has_value(); }
    void setLinkTypeId(const int64_t& value)
    {
        m_linkTypeId = value;
    }
    void setLinkTypeId(int64_t&& value)
    {
        m_linkTypeId = std::move(value);
    }
    void clearLinkTypeId()
    {
        m_linkTypeId = std::nullopt;
    }

    // Поле: «sourceItemId» Исходный элемент
    const std::optional<int64_t>& sourceItemId() const { return m_sourceItemId; }
    bool hasSourceItemId() const { return m_sourceItemId.has_value(); }
    void setSourceItemId(const int64_t& value)
    {
        m_sourceItemId = value;
    }
    void setSourceItemId(int64_t&& value)
    {
        m_sourceItemId = std::move(value);
    }
    void clearSourceItemId()
    {
        m_sourceItemId = std::nullopt;
    }

    // Поле: «destinationItemId» Целевой элемент
    const std::optional<int64_t>& destinationItemId() const { return m_destinationItemId; }
    bool hasDestinationItemId() const { return m_destinationItemId.has_value(); }
    void setDestinationItemId(const int64_t& value)
    {
        m_destinationItemId = value;
    }
    void setDestinationItemId(int64_t&& value)
    {
        m_destinationItemId = std::move(value);
    }
    void clearDestinationItemId()
    {
        m_destinationItemId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ItemLink& other) const;
    bool operator!=(const ItemLink& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ItemLink& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_linkTypeId;
    std::optional<int64_t> m_sourceItemId;
    std::optional<int64_t> m_destinationItemId;
};

inline std::ostream& operator<<(std::ostream& os, const ItemLink& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto