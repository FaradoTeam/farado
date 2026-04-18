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
 * @brief Item DTO
 */
class Item final
{
public:
    using Ptr = std::shared_ptr<Item>;
    using ConstPtr = std::shared_ptr<const Item>;

public:
    Item() = default;
    explicit Item(const nlohmann::json& json);

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

    // Поле: «parentId» Идентификатор родительского элемента
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

    // Поле: «stateId» Идентификатор текущего состояния
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

    // Поле: «phaseId» Идентификатор фазы
    const std::optional<int64_t>& phaseId() const { return m_phaseId; }
    bool hasPhaseId() const { return m_phaseId.has_value(); }
    void setPhaseId(const int64_t& value)
    {
        m_phaseId = value;
    }
    void setPhaseId(int64_t&& value)
    {
        m_phaseId = std::move(value);
    }
    void clearPhaseId()
    {
        m_phaseId = std::nullopt;
    }

    // Поле: «caption» Название элемента
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

    // Поле: «content» Содержимое элемента
    const std::optional<std::string>& content() const { return m_content; }
    bool hasContent() const { return m_content.has_value(); }
    void setContent(const std::string& value)
    {
        m_content = value;
    }
    void setContent(std::string&& value)
    {
        m_content = std::move(value);
    }
    void clearContent()
    {
        m_content = std::nullopt;
    }

    // Поле: «isDeleted» Флаг удаления (мягкое удаление)
    const std::optional<bool>& isDeleted() const { return m_isDeleted; }
    bool hasIsDeleted() const { return m_isDeleted.has_value(); }
    void setIsDeleted(const bool& value)
    {
        m_isDeleted = value;
    }
    void setIsDeleted(bool&& value)
    {
        m_isDeleted = std::move(value);
    }
    void clearIsDeleted()
    {
        m_isDeleted = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Item& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemTypeId;
    std::optional<int64_t> m_parentId;
    std::optional<int64_t> m_stateId;
    std::optional<int64_t> m_phaseId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_content;
    std::optional<bool> m_isDeleted;
};

inline std::ostream& operator<<(std::ostream& os, const Item& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto