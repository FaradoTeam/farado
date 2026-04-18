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
 * @brief ItemType DTO
 */
class ItemType final
{
public:
    using Ptr = std::shared_ptr<ItemType>;
    using ConstPtr = std::shared_ptr<const ItemType>;

public:
    ItemType() = default;
    explicit ItemType(const nlohmann::json& json);

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

    // Поле: «workflowId» Идентификатор рабочего процесса
    const std::optional<int64_t>& workflowId() const { return m_workflowId; }
    bool hasWorkflowId() const { return m_workflowId.has_value(); }
    void setWorkflowId(const int64_t& value)
    {
        m_workflowId = value;
    }
    void setWorkflowId(int64_t&& value)
    {
        m_workflowId = std::move(value);
    }
    void clearWorkflowId()
    {
        m_workflowId = std::nullopt;
    }

    // Поле: «defaultStateId» Состояние по умолчанию для новых элементов
    const std::optional<int64_t>& defaultStateId() const { return m_defaultStateId; }
    bool hasDefaultStateId() const { return m_defaultStateId.has_value(); }
    void setDefaultStateId(const int64_t& value)
    {
        m_defaultStateId = value;
    }
    void setDefaultStateId(int64_t&& value)
    {
        m_defaultStateId = std::move(value);
    }
    void clearDefaultStateId()
    {
        m_defaultStateId = std::nullopt;
    }

    // Поле: «caption» Название типа
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

    // Поле: «kind» Вид элемента
    const std::optional<std::string>& kind() const { return m_kind; }
    bool hasKind() const { return m_kind.has_value(); }
    void setKind(const std::string& value)
    {
        m_kind = value;
    }
    void setKind(std::string&& value)
    {
        m_kind = std::move(value);
    }
    void clearKind()
    {
        m_kind = std::nullopt;
    }

    // Поле: «defaultContent» Содержимое по умолчанию (шаблон для создания новых item)
    const std::optional<std::string>& defaultContent() const { return m_defaultContent; }
    bool hasDefaultContent() const { return m_defaultContent.has_value(); }
    void setDefaultContent(const std::string& value)
    {
        m_defaultContent = value;
    }
    void setDefaultContent(std::string&& value)
    {
        m_defaultContent = std::move(value);
    }
    void clearDefaultContent()
    {
        m_defaultContent = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ItemType& other) const;
    bool operator!=(const ItemType& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ItemType& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_workflowId;
    std::optional<int64_t> m_defaultStateId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_kind;
    std::optional<std::string> m_defaultContent;
};

inline std::ostream& operator<<(std::ostream& os, const ItemType& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto