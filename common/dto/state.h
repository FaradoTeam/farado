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
 * @brief State DTO
 */
class State final
{
public:
    using Ptr = std::shared_ptr<State>;
    using ConstPtr = std::shared_ptr<const State>;

public:
    State() = default;
    explicit State(const nlohmann::json& json);

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

    // Поле: «caption» Название состояния
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

    // Поле: «description» Описание состояния
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

    // Поле: «weight» Вес состояния (для метрик)
    const std::optional<int64_t>& weight() const { return m_weight; }
    bool hasWeight() const { return m_weight.has_value(); }
    void setWeight(const int64_t& value)
    {
        m_weight = value;
    }
    void setWeight(int64_t&& value)
    {
        m_weight = std::move(value);
    }
    void clearWeight()
    {
        m_weight = std::nullopt;
    }

    // Поле: «orderNumber» Порядковый номер для сортировки
    const std::optional<int64_t>& orderNumber() const { return m_orderNumber; }
    bool hasOrderNumber() const { return m_orderNumber.has_value(); }
    void setOrderNumber(const int64_t& value)
    {
        m_orderNumber = value;
    }
    void setOrderNumber(int64_t&& value)
    {
        m_orderNumber = std::move(value);
    }
    void clearOrderNumber()
    {
        m_orderNumber = std::nullopt;
    }

    // Поле: «isArchive» Флаг архивного состояния (редко отображается)
    const std::optional<bool>& isArchive() const { return m_isArchive; }
    bool hasIsArchive() const { return m_isArchive.has_value(); }
    void setIsArchive(const bool& value)
    {
        m_isArchive = value;
    }
    void setIsArchive(bool&& value)
    {
        m_isArchive = std::move(value);
    }
    void clearIsArchive()
    {
        m_isArchive = std::nullopt;
    }

    // Поле: «isQueue» Флаг очереди (для определения активных работ)
    const std::optional<bool>& isQueue() const { return m_isQueue; }
    bool hasIsQueue() const { return m_isQueue.has_value(); }
    void setIsQueue(const bool& value)
    {
        m_isQueue = value;
    }
    void setIsQueue(bool&& value)
    {
        m_isQueue = std::move(value);
    }
    void clearIsQueue()
    {
        m_isQueue = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const State& other) const;
    bool operator!=(const State& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const State& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_workflowId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
    std::optional<int64_t> m_weight;
    std::optional<int64_t> m_orderNumber;
    std::optional<bool> m_isArchive;
    std::optional<bool> m_isQueue;
};

inline std::ostream& operator<<(std::ostream& os, const State& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto