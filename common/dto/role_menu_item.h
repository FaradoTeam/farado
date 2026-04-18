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
 * @brief RoleMenuItem DTO
 */
class RoleMenuItem final
{
public:
    using Ptr = std::shared_ptr<RoleMenuItem>;
    using ConstPtr = std::shared_ptr<const RoleMenuItem>;

public:
    RoleMenuItem() = default;
    explicit RoleMenuItem(const nlohmann::json& json);

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

    // Поле: «caption» Название пункта меню
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

    // Поле: «link» Ссылка (URL)
    const std::optional<std::string>& link() const { return m_link; }
    bool hasLink() const { return m_link.has_value(); }
    void setLink(const std::string& value)
    {
        m_link = value;
    }
    void setLink(std::string&& value)
    {
        m_link = std::move(value);
    }
    void clearLink()
    {
        m_link = std::nullopt;
    }

    // Поле: «icon» Значок пункта меню
    const std::optional<std::string>& icon() const { return m_icon; }
    bool hasIcon() const { return m_icon.has_value(); }
    void setIcon(const std::string& value)
    {
        m_icon = value;
    }
    void setIcon(std::string&& value)
    {
        m_icon = std::move(value);
    }
    void clearIcon()
    {
        m_icon = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const RoleMenuItem& other) const;
    bool operator!=(const RoleMenuItem& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const RoleMenuItem& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_roleId;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_link;
    std::optional<std::string> m_icon;
};

inline std::ostream& operator<<(std::ostream& os, const RoleMenuItem& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto