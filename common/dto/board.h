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
 * @brief Board DTO
 */
class Board final
{
public:
    using Ptr = std::shared_ptr<Board>;
    using ConstPtr = std::shared_ptr<const Board>;

public:
    Board() = default;
    explicit Board(const nlohmann::json& json);

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

    // Поле: «projectId» Идентификатор проекта
    const std::optional<int64_t>& projectId() const { return m_projectId; }
    bool hasProjectId() const { return m_projectId.has_value(); }
    void setProjectId(const int64_t& value)
    {
        m_projectId = value;
    }
    void setProjectId(int64_t&& value)
    {
        m_projectId = std::move(value);
    }
    void clearProjectId()
    {
        m_projectId = std::nullopt;
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

    // Поле: «caption» Название доски
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

    // Поле: «description» Описание доски
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

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Board& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_workflowId;
    std::optional<int64_t> m_projectId;
    std::optional<int64_t> m_phaseId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
};

inline std::ostream& operator<<(std::ostream& os, const Board& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto