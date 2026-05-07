#include <regex>

#include <cpprest/uri.h>

#include "common/dto/state.h"
#include "common/helpers/json_helper.hpp"
#include "common/log/log.h"

#include "states_handler.h"

namespace server
{
namespace handlers
{

StatesHandler::StatesHandler(
    std::shared_ptr<services::IStateService> stateService
)
    : m_stateService(std::move(stateService))
{
    if (!m_stateService)
    {
        LOG_WARN << "StatesHandler инициализирован без StateService";
    }
}

void StatesHandler::handleGetStates(
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

    std::optional<int64_t> workflowId;
    if (params.count("workflowId"))
        workflowId = std::stoll(params["workflowId"]);

    auto statesPage = m_stateService->states(page, pageSize, workflowId);

    web::json::value response;
    web::json::value items = web::json::value::array();

    for (size_t i = 0; i < statesPage.states.size(); ++i)
    {
        items[i] = dto::toWebJson(statesPage.states[i].toJson());
    }

    response["items"] = items;
    response["totalCount"] = web::json::value::number(statesPage.totalCount);
    response["page"] = web::json::value::number(page);
    response["pageSize"] = web::json::value::number(pageSize);

    request.reply(web::http::status_codes::OK, response);
}

void StatesHandler::handleGetState(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    int64_t id = extractStateIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid state ID");
        request.reply(resp);
        return;
    }

    auto state = m_stateService->state(id);
    if (!state)
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "State not found");
        request.reply(resp);
        return;
    }

    request.reply(
        web::http::status_codes::OK,
        dto::toWebJson(state->toJson())
    );
}

void StatesHandler::handleCreateState(
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
                    dto::State state(dto::toNlohmannJson(jsonBody));

                    if (!state.workflowId || !state.caption || state.caption->empty())
                    {
                        web::http::http_response resp(
                            web::http::status_codes::BadRequest
                        );
                        sendErrorResponse(
                            resp, 400,
                            "workflowId and caption are required"
                        );
                        request.reply(resp);
                        return;
                    }

                    auto created = m_stateService->createState(state);
                    if (!created)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::BadRequest
                        );
                        sendErrorResponse(
                            resp, 400,
                            "Failed to create state. Check workflow exists."
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
                    web::http::http_response resp(
                        web::http::status_codes::BadRequest
                    );
                    sendErrorResponse(
                        resp, 400,
                        std::string("Invalid request: ") + e.what()
                    );
                    request.reply(resp);
                }
            }
        )
        .wait();
}

void StatesHandler::handleUpdateState(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractStateIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid state ID");
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
                    nlohmann::json nlohmannJson = dto::toNlohmannJson(jsonBody);
                    nlohmannJson["id"] = id;
                    dto::State state(nlohmannJson);

                    auto updated = m_stateService->updateState(state);
                    if (!updated)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::NotFound
                        );
                        sendErrorResponse(
                            resp, 404,
                            "State not found or update failed"
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
                        resp, 400,
                        std::string("Invalid request: ") + e.what()
                    );
                    request.reply(resp);
                }
            }
        )
        .wait();
}

void StatesHandler::handleDeleteState(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractStateIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid state ID");
        request.reply(resp);
        return;
    }

    auto result = m_stateService->deleteState(id);
    if (!result.success)
    {
        web::http::http_response resp(
            static_cast<web::http::status_code>(result.errorCode)
        );
        sendErrorResponse(resp, result.errorCode, result.errorMessage);
        request.reply(resp);
        return;
    }

    request.reply(web::http::status_codes::NoContent);
}

int64_t StatesHandler::extractStateIdFromPath(
    const web::http::http_request& request
)
{
    std::string path = web::uri::decode(request.relative_uri().path());
    std::regex pattern(R"(/api/states/(\d+))");
    std::smatch matches;
    if (std::regex_match(path, matches, pattern) && matches.size() > 1)
    {
        return std::stoll(matches[1].str());
    }
    return -1;
}

std::map<std::string, std::string> StatesHandler::extractQueryParams(
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

void StatesHandler::sendErrorResponse(
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
