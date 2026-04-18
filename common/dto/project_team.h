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
 * @brief ProjectTeam DTO
 */
class ProjectTeam final
{
public:
    using Ptr = std::shared_ptr<ProjectTeam>;
    using ConstPtr = std::shared_ptr<const ProjectTeam>;

public:
    ProjectTeam() = default;
    explicit ProjectTeam(const nlohmann::json& json);

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

    // Поле: «teamId» Идентификатор команды
    const std::optional<int64_t>& teamId() const { return m_teamId; }
    bool hasTeamId() const { return m_teamId.has_value(); }
    void setTeamId(const int64_t& value)
    {
        m_teamId = value;
    }
    void setTeamId(int64_t&& value)
    {
        m_teamId = std::move(value);
    }
    void clearTeamId()
    {
        m_teamId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const ProjectTeam& other) const;
    bool operator!=(const ProjectTeam& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const ProjectTeam& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_projectId;
    std::optional<int64_t> m_teamId;
};

inline std::ostream& operator<<(std::ostream& os, const ProjectTeam& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto