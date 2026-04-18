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
 * @brief UserTeamRole DTO
 */
class UserTeamRole final
{
public:
    using Ptr = std::shared_ptr<UserTeamRole>;
    using ConstPtr = std::shared_ptr<const UserTeamRole>;

public:
    UserTeamRole() = default;
    explicit UserTeamRole(const nlohmann::json& json);

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

    // Поле: «userId» Идентификатор пользователя
    const std::optional<int64_t>& userId() const { return m_userId; }
    bool hasUserId() const { return m_userId.has_value(); }
    void setUserId(const int64_t& value)
    {
        m_userId = value;
    }
    void setUserId(int64_t&& value)
    {
        m_userId = std::move(value);
    }
    void clearUserId()
    {
        m_userId = std::nullopt;
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

    // Поле: «roleId» Идентификатор роли
    const std::optional<int64_t>& roleId() const { return m_roleId; }
    bool hasRoleId() const { return m_roleId.has_value(); }
    void setRoleId(const int64_t& value)
    {
        m_roleId = value;
    }
    void setRoleId(int64_t&& value)
    {
        m_roleId = std::move(value);
    }
    void clearRoleId()
    {
        m_roleId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const UserTeamRole& other) const;
    bool operator!=(const UserTeamRole& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const UserTeamRole& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_userId;
    std::optional<int64_t> m_teamId;
    std::optional<int64_t> m_roleId;
};

inline std::ostream& operator<<(std::ostream& os, const UserTeamRole& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto