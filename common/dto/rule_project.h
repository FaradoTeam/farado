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
 * @brief RuleProject DTO
 */
class RuleProject final
{
public:
    using Ptr = std::shared_ptr<RuleProject>;
    using ConstPtr = std::shared_ptr<const RuleProject>;

public:
    RuleProject() = default;
    explicit RuleProject(const nlohmann::json& json);

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

    // Поле: «ruleId» Идентификатор правила
    const std::optional<int64_t>& ruleId() const { return m_ruleId; }
    bool hasRuleId() const { return m_ruleId.has_value(); }
    void setRuleId(const int64_t& value)
    {
        m_ruleId = value;
    }
    void setRuleId(int64_t&& value)
    {
        m_ruleId = std::move(value);
    }
    void clearRuleId()
    {
        m_ruleId = std::nullopt;
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

    // Поле: «isReader» Право на просмотр элементов (items) проекта
    const std::optional<bool>& isReader() const { return m_isReader; }
    bool hasIsReader() const { return m_isReader.has_value(); }
    void setIsReader(const bool& value)
    {
        m_isReader = value;
    }
    void setIsReader(bool&& value)
    {
        m_isReader = std::move(value);
    }
    void clearIsReader()
    {
        m_isReader = std::nullopt;
    }

    // Поле: «isWriter» Право на создание и изменение элементов (items) проекта
    const std::optional<bool>& isWriter() const { return m_isWriter; }
    bool hasIsWriter() const { return m_isWriter.has_value(); }
    void setIsWriter(const bool& value)
    {
        m_isWriter = value;
    }
    void setIsWriter(bool&& value)
    {
        m_isWriter = std::move(value);
    }
    void clearIsWriter()
    {
        m_isWriter = std::nullopt;
    }

    // Поле: «isProjectEditor» Право на создание и изменение подпроектов
    const std::optional<bool>& isProjectEditor() const { return m_isProjectEditor; }
    bool hasIsProjectEditor() const { return m_isProjectEditor.has_value(); }
    void setIsProjectEditor(const bool& value)
    {
        m_isProjectEditor = value;
    }
    void setIsProjectEditor(bool&& value)
    {
        m_isProjectEditor = std::move(value);
    }
    void clearIsProjectEditor()
    {
        m_isProjectEditor = std::nullopt;
    }

    // Поле: «isPhaseEditor» Право на создание и изменение фаз
    const std::optional<bool>& isPhaseEditor() const { return m_isPhaseEditor; }
    bool hasIsPhaseEditor() const { return m_isPhaseEditor.has_value(); }
    void setIsPhaseEditor(const bool& value)
    {
        m_isPhaseEditor = value;
    }
    void setIsPhaseEditor(bool&& value)
    {
        m_isPhaseEditor = std::move(value);
    }
    void clearIsPhaseEditor()
    {
        m_isPhaseEditor = std::nullopt;
    }

    // Поле: «isBoardEditor» Право на создание и изменение досок
    const std::optional<bool>& isBoardEditor() const { return m_isBoardEditor; }
    bool hasIsBoardEditor() const { return m_isBoardEditor.has_value(); }
    void setIsBoardEditor(const bool& value)
    {
        m_isBoardEditor = value;
    }
    void setIsBoardEditor(bool&& value)
    {
        m_isBoardEditor = std::move(value);
    }
    void clearIsBoardEditor()
    {
        m_isBoardEditor = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const RuleProject& other) const;
    bool operator!=(const RuleProject& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const RuleProject& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_ruleId;
    std::optional<int64_t> m_projectId;
    std::optional<bool> m_isReader;
    std::optional<bool> m_isWriter;
    std::optional<bool> m_isProjectEditor;
    std::optional<bool> m_isPhaseEditor;
    std::optional<bool> m_isBoardEditor;
};

inline std::ostream& operator<<(std::ostream& os, const RuleProject& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto