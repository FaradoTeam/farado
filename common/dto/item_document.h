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
 * @brief ItemDocument DTO
 */
class ItemDocument final
{
public:
    using Ptr = std::shared_ptr<ItemDocument>;
    using ConstPtr = std::shared_ptr<const ItemDocument>;

public:
    ItemDocument() = default;
    explicit ItemDocument(const nlohmann::json& json);

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

    // Поле: «itemId» Идентификатор элемента
    const std::optional<int64_t>& itemId() const { return m_itemId; }
    bool hasItemId() const { return m_itemId.has_value(); }
    void setItemId(const int64_t& value)
    {
        m_itemId = value;
    }
    void setItemId(int64_t&& value)
    {
        m_itemId = std::move(value);
    }
    void clearItemId()
    {
        m_itemId = std::nullopt;
    }

    // Поле: «documentId» Идентификатор документа
    const std::optional<int64_t>& documentId() const { return m_documentId; }
    bool hasDocumentId() const { return m_documentId.has_value(); }
    void setDocumentId(const int64_t& value)
    {
        m_documentId = value;
    }
    void setDocumentId(int64_t&& value)
    {
        m_documentId = std::move(value);
    }
    void clearDocumentId()
    {
        m_documentId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ItemDocument& other) const;
    bool operator!=(const ItemDocument& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ItemDocument& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_itemId;
    std::optional<int64_t> m_documentId;
};

inline std::ostream& operator<<(std::ostream& os, const ItemDocument& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto