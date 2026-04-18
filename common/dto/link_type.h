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
 * @brief LinkType DTO
 */
class LinkType final
{
public:
    using Ptr = std::shared_ptr<LinkType>;
    using ConstPtr = std::shared_ptr<const LinkType>;

public:
    LinkType() = default;
    explicit LinkType(const nlohmann::json& json);

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

    // Поле: «sourceItemTypeId» Исходный тип элемента
    const std::optional<int64_t>& sourceItemTypeId() const { return m_sourceItemTypeId; }
    bool hasSourceItemTypeId() const { return m_sourceItemTypeId.has_value(); }
    void setSourceItemTypeId(const int64_t& value)
    {
        m_sourceItemTypeId = value;
    }
    void setSourceItemTypeId(int64_t&& value)
    {
        m_sourceItemTypeId = std::move(value);
    }
    void clearSourceItemTypeId()
    {
        m_sourceItemTypeId = std::nullopt;
    }

    // Поле: «destinationItemTypeId» Целевой тип элемента
    const std::optional<int64_t>& destinationItemTypeId() const { return m_destinationItemTypeId; }
    bool hasDestinationItemTypeId() const { return m_destinationItemTypeId.has_value(); }
    void setDestinationItemTypeId(const int64_t& value)
    {
        m_destinationItemTypeId = value;
    }
    void setDestinationItemTypeId(int64_t&& value)
    {
        m_destinationItemTypeId = std::move(value);
    }
    void clearDestinationItemTypeId()
    {
        m_destinationItemTypeId = std::nullopt;
    }

    // Поле: «isBidirectional» Флаг двунаправленной связи
    const std::optional<bool>& isBidirectional() const { return m_isBidirectional; }
    bool hasIsBidirectional() const { return m_isBidirectional.has_value(); }
    void setIsBidirectional(const bool& value)
    {
        m_isBidirectional = value;
    }
    void setIsBidirectional(bool&& value)
    {
        m_isBidirectional = std::move(value);
    }
    void clearIsBidirectional()
    {
        m_isBidirectional = std::nullopt;
    }

    // Поле: «caption» Название типа связи
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

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const LinkType& other) const;
    bool operator!=(const LinkType& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const LinkType& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_sourceItemTypeId;
    std::optional<int64_t> m_destinationItemTypeId;
    std::optional<bool> m_isBidirectional;
    std::optional<std::string> m_caption;
};

inline std::ostream& operator<<(std::ostream& os, const LinkType& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto