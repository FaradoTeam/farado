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
 * @brief PageResponse DTO
 */
class PageResponse final
{
public:
    using Ptr = std::shared_ptr<PageResponse>;
    using ConstPtr = std::shared_ptr<const PageResponse>;

public:
    PageResponse() = default;
    explicit PageResponse(const nlohmann::json& json);

    // Поле: «items» 
    const std::optional<std::vector<std::string>>& items() const { return m_items; }
    bool hasItems() const { return m_items.has_value(); }
    void setItems(const std::vector<std::string>& value)
    {
        m_items = value;
    }
    void setItems(std::vector<std::string>&& value)
    {
        m_items = std::move(value);
    }
    void clearItems()
    {
        m_items = std::nullopt;
    }

    // Поле: «totalCount» 
    const std::optional<int64_t>& totalCount() const { return m_totalCount; }
    bool hasTotalCount() const { return m_totalCount.has_value(); }
    void setTotalCount(const int64_t& value)
    {
        m_totalCount = value;
    }
    void setTotalCount(int64_t&& value)
    {
        m_totalCount = std::move(value);
    }
    void clearTotalCount()
    {
        m_totalCount = std::nullopt;
    }

    // Поле: «page» 
    const std::optional<int64_t>& page() const { return m_page; }
    bool hasPage() const { return m_page.has_value(); }
    void setPage(const int64_t& value)
    {
        m_page = value;
    }
    void setPage(int64_t&& value)
    {
        m_page = std::move(value);
    }
    void clearPage()
    {
        m_page = std::nullopt;
    }

    // Поле: «pageSize» 
    const std::optional<int64_t>& pageSize() const { return m_pageSize; }
    bool hasPageSize() const { return m_pageSize.has_value(); }
    void setPageSize(const int64_t& value)
    {
        m_pageSize = value;
    }
    void setPageSize(int64_t&& value)
    {
        m_pageSize = std::move(value);
    }
    void clearPageSize()
    {
        m_pageSize = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const PageResponse& other) const;
    bool operator!=(const PageResponse& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const PageResponse& dto);

private:
    std::optional<std::vector<std::string>> m_items;
    std::optional<int64_t> m_totalCount;
    std::optional<int64_t> m_page;
    std::optional<int64_t> m_pageSize;
};

inline std::ostream& operator<<(std::ostream& os, const PageResponse& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto