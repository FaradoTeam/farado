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
 * @brief FieldType DTO
 */
class FieldType final
{
public:
    using Ptr = std::shared_ptr<FieldType>;
    using ConstPtr = std::shared_ptr<const FieldType>;

public:
    FieldType() = default;
    explicit FieldType(const nlohmann::json& json);

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

    // Поле: «caption» Название поля
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

    // Поле: «description» Описание поля
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

    // Поле: «valueType» Тип значения
    const std::optional<std::string>& valueType() const { return m_valueType; }
    bool hasValueType() const { return m_valueType.has_value(); }
    void setValueType(const std::string& value)
    {
        m_valueType = value;
    }
    void setValueType(std::string&& value)
    {
        m_valueType = std::move(value);
    }
    void clearValueType()
    {
        m_valueType = std::nullopt;
    }

    // Поле: «isBoardVisible» Отображать поле на канбан-доске
    const std::optional<bool>& isBoardVisible() const { return m_isBoardVisible; }
    bool hasIsBoardVisible() const { return m_isBoardVisible.has_value(); }
    void setIsBoardVisible(const bool& value)
    {
        m_isBoardVisible = value;
    }
    void setIsBoardVisible(bool&& value)
    {
        m_isBoardVisible = std::move(value);
    }
    void clearIsBoardVisible()
    {
        m_isBoardVisible = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const FieldType& other) const;
    bool operator!=(const FieldType& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const FieldType& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemTypeId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
    std::optional<std::string> m_valueType;
    std::optional<bool> m_isBoardVisible;
};

inline std::ostream& operator<<(std::ostream& os, const FieldType& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto