#include <chrono>
#include <ctime>

#include <nlohmann/json.hpp>

#include "common/helpers/time_helpers.h"

#include "page_response.h"

namespace dto
{

PageResponse::PageResponse(const nlohmann::json& json)
{
    fromJson(json);
}

nlohmann::json PageResponse::toJson() const
{
    nlohmann::json result;

    // items
    if (m_items.has_value())
    {
        if (!m_items.value().empty())
        {
            nlohmann::json arr = nlohmann::json::array();
            for (const auto& item : m_items.value())
            {
                arr.push_back(item);
            }
            result["items"] = arr;
        }
    }
    // totalCount
    if (m_totalCount.has_value())
    {
        result["total_count"] = m_totalCount.value();
    }
    // page
    if (m_page.has_value())
    {
        result["page"] = m_page.value();
    }
    // pageSize
    if (m_pageSize.has_value())
    {
        result["page_size"] = m_pageSize.value();
    }

    return result;
}

bool PageResponse::fromJson(const nlohmann::json& json)
{
    bool success = true;

    // items
    if (json.contains("items") && !json["items"].is_null())
    {
        try
        {
            std::vector<std::string> vec;
            for (const auto& item : json["items"])
            {
                vec.push_back(item.get<std::string>());
            }
            m_items = std::move(vec);
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_items = std::nullopt;
    }
    // totalCount
    if (json.contains("total_count") && !json["total_count"].is_null())
    {
        try
        {
            m_totalCount = json["total_count"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_totalCount = std::nullopt;
    }
    // page
    if (json.contains("page") && !json["page"].is_null())
    {
        try
        {
            m_page = json["page"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_page = std::nullopt;
    }
    // pageSize
    if (json.contains("page_size") && !json["page_size"].is_null())
    {
        try
        {
            m_pageSize = json["page_size"].get<int64_t>();
        }
        catch (const std::exception& e)
        {
            success = false;
        }
    }
    else
    {
        m_pageSize = std::nullopt;
    }

    return success;
}

bool PageResponse::isValid() const
{

    // Дополнительные проверки для непустых значений

    return true;
}

std::string PageResponse::validationError() const
{


    return "";
}

bool PageResponse::operator==(const PageResponse& other) const
{
    return
        m_items == other.m_items
        && m_totalCount == other.m_totalCount
        && m_page == other.m_page
        && m_pageSize == other.m_pageSize
;
}

} // namespace dto