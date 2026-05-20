#include <regex>

#include <cpprest/uri.h>

#include "common/dto/item_type.h"
#include "common/helpers/json_helper.hpp"
#include "common/log/log.h"

#include "item_types_handler.h"

namespace server
{
namespace handlers
{

ItemTypesHandler::ItemTypesHandler(
    std::shared_ptr<services::IItemTypeService> itemTypeService
)
    : m_itemTypeService(std::move(itemTypeService))
{
    if (!m_itemTypeService)
    {
        LOG_WARN << "ItemTypesHandler инициализирован без ItemTypeService";
    }
}

void ItemTypesHandler::handleGetItemTypes(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    auto params = extractQueryParams(request);

    int page = 1;
    if (params.count("page"))
        page = std::stoi(params["page"]);

    int pageSize = 20;
    if (params.count("pageSize"))
        pageSize = std::stoi(params["pageSize"]);

    std::optional<int64_t> workflowId = std::nullopt;
    if (params.count("workflowId"))
        workflowId = std::stoll(params["workflowId"]);

    std::optional<std::string> kind = std::nullopt;
    if (params.count("kind"))
        kind = params["kind"];

    std::string searchCaption = "";
    if (params.count("searchCaption"))
        searchCaption = params["searchCaption"];

    auto itemTypesPage = m_itemTypeService->itemTypes(
        page, pageSize, workflowId, kind, searchCaption
    );

    web::json::value response;
    web::json::value items = web::json::value::array();

    for (size_t i = 0; i < itemTypesPage.itemTypes.size(); ++i)
    {
        items[i] = dto::toWebJson(itemTypesPage.itemTypes[i].toJson());
    }

    response["items"] = items;
    response["totalCount"] = web::json::value::number(itemTypesPage.totalCount);
    response["page"] = web::json::value::number(page);
    response["pageSize"] = web::json::value::number(pageSize);

    request.reply(web::http::status_codes::OK, response);
}

void ItemTypesHandler::handleGetItemType(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    int64_t id = extractItemTypeIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid item type ID");
        request.reply(resp);
        return;
    }

    auto itemType = m_itemTypeService->itemType(id);
    if (!itemType)
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Item type not found");
        request.reply(resp);
        return;
    }

    request.reply(
        web::http::status_codes::OK,
        dto::toWebJson(itemType->toJson())
    );
}

void ItemTypesHandler::handleCreateItemType(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    request
        .extract_json()
        .then(
            [this, request](pplx::task<web::json::value> task)
            {
                try
                {
                    auto jsonBody = task.get();
                    auto nlohmannJson = dto::toNlohmannJson(jsonBody);
                    dto::ItemType itemType(nlohmannJson);

                    auto created = m_itemTypeService->createItemType(itemType);
                    if (!created)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::BadRequest
                        );
                        sendErrorResponse(
                            resp,
                            400,
                            "Invalid item type data or could not create item type"
                        );
                        request.reply(resp);
                        return;
                    }

                    request.reply(
                        web::http::status_codes::Created,
                        dto::toWebJson(created->toJson())
                    );
                }
                catch (const std::exception& e)
                {
                    web::http::http_response resp(web::http::status_codes::BadRequest);
                    sendErrorResponse(
                        resp,
                        400,
                        std::string("Invalid request: ") + e.what()
                    );
                    request.reply(resp);
                }
            }
        )
        .wait();
}

void ItemTypesHandler::handleUpdateItemType(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractItemTypeIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid item type ID");
        request.reply(resp);
        return;
    }

    request
        .extract_json()
        .then(
            [this, request, id](pplx::task<web::json::value> task)
            {
                try
                {
                    auto jsonBody = task.get();
                    auto nlohmannJson = dto::toNlohmannJson(jsonBody);
                    nlohmannJson["id"] = id;
                    dto::ItemType itemType(nlohmannJson);

                    auto updated = m_itemTypeService->updateItemType(itemType);
                    if (!updated)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::NotFound
                        );
                        sendErrorResponse(
                            resp,
                            404,
                            "Item type not found or update failed"
                        );
                        request.reply(resp);
                        return;
                    }

                    request.reply(
                        web::http::status_codes::OK,
                        dto::toWebJson(updated->toJson())
                    );
                }
                catch (const std::exception& e)
                {
                    web::http::http_response resp(
                        web::http::status_codes::BadRequest
                    );
                    sendErrorResponse(
                        resp,
                        400,
                        std::string("Invalid request: ") + e.what()
                    );
                    request.reply(resp);
                }
            }
        )
        .wait();
}

void ItemTypesHandler::handleDeleteItemType(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractItemTypeIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid item type ID");
        request.reply(resp);
        return;
    }

    if (m_itemTypeService->deleteItemType(id))
    {
        request.reply(web::http::status_codes::NoContent);
    }
    else
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Item type not found");
        request.reply(resp);
    }
}

int64_t ItemTypesHandler::extractItemTypeIdFromPath(
    const web::http::http_request& request
)
{
    std::string path = web::uri::decode(request.relative_uri().path());
    std::regex pattern(R"(/api/item-types/(\d+))");
    std::smatch matches;
    if (std::regex_match(path, matches, pattern) && matches.size() > 1)
    {
        return std::stoll(matches[1].str());
    }
    return -1;
}

std::map<std::string, std::string> ItemTypesHandler::extractQueryParams(
    const web::http::http_request& request
)
{
    std::map<std::string, std::string> params;
    auto query = web::uri::split_query(request.request_uri().query());
    for (const auto& p : query)
    {
        params[p.first] = p.second;
    }
    return params;
}

void ItemTypesHandler::sendErrorResponse(
    web::http::http_response& response,
    int code,
    const std::string& message
)
{
    web::json::value error;
    error["code"] = web::json::value::number(code);
    error["message"] = web::json::value::string(message);
    response.set_body(error);
}

} // namespace handlers
} // namespace server
