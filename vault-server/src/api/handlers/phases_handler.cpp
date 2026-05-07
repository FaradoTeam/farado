#include <regex>

#include <cpprest/uri.h>

#include "common/dto/phase.h"
#include "common/helpers/json_helper.hpp"
#include "common/log/log.h"

#include "phases_handler.h"

namespace server
{
namespace handlers
{

PhasesHandler::PhasesHandler(
    std::shared_ptr<services::IPhaseService> phaseService
)
    : m_phaseService(std::move(phaseService))
{
    if (!m_phaseService)
    {
        LOG_WARN << "PhasesHandler инициализирован без PhaseService";
    }
}

void PhasesHandler::handleGetPhases(
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

    std::optional<int64_t> projectId = std::nullopt;
    if (params.count("projectId"))
        projectId = std::stoll(params["projectId"]);

    std::optional<bool> isArchive = std::nullopt;
    if (params.count("isArchive"))
        isArchive = params["isArchive"] == "true";

    auto phasesPage = m_phaseService->phases(page, pageSize, projectId, isArchive);

    web::json::value response;
    web::json::value items = web::json::value::array();

    for (size_t i = 0; i < phasesPage.phases.size(); ++i)
    {
        items[i] = dto::toWebJson(phasesPage.phases[i].toJson());
    }

    response["items"] = items;
    response["totalCount"] = web::json::value::number(phasesPage.totalCount);
    response["page"] = web::json::value::number(page);
    response["pageSize"] = web::json::value::number(pageSize);

    request.reply(web::http::status_codes::OK, response);
}

void PhasesHandler::handleGetPhase(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    int64_t id = extractPhaseIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid phase ID");
        request.reply(resp);
        return;
    }

    auto phase = m_phaseService->phase(id);
    if (!phase)
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Phase not found");
        request.reply(resp);
        return;
    }

    request.reply(
        web::http::status_codes::OK,
        dto::toWebJson(phase->toJson())
    );
}

void PhasesHandler::handleCreatePhase(
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
                    dto::Phase phase(nlohmannJson);

                    auto created = m_phaseService->createPhase(phase);
                    if (!created)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::BadRequest
                        );
                        sendErrorResponse(
                            resp,
                            400,
                            "Invalid phase data or could not create phase"
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

void PhasesHandler::handleUpdatePhase(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractPhaseIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid phase ID");
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
                    dto::Phase phase(nlohmannJson);

                    auto updated = m_phaseService->updatePhase(phase);
                    if (!updated)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::NotFound
                        );
                        sendErrorResponse(
                            resp,
                            404,
                            "Phase not found or update failed"
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

void PhasesHandler::handleDeletePhase(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractPhaseIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid phase ID");
        request.reply(resp);
        return;
    }

    if (m_phaseService->archivePhase(id))
    {
        request.reply(web::http::status_codes::NoContent);
    }
    else
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Phase not found");
        request.reply(resp);
    }
}

int64_t PhasesHandler::extractPhaseIdFromPath(
    const web::http::http_request& request
)
{
    std::string path = web::uri::decode(request.relative_uri().path());
    std::regex pattern(R"(/api/phases/(\d+))");
    std::smatch matches;
    if (std::regex_match(path, matches, pattern) && matches.size() > 1)
    {
        return std::stoll(matches[1].str());
    }
    return -1;
}

std::map<std::string, std::string> PhasesHandler::extractQueryParams(
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

void PhasesHandler::sendErrorResponse(
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
