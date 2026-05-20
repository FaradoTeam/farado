#include <regex>

#include <cpprest/uri.h>

#include "common/dto/project.h"
#include "common/helpers/json_helper.hpp"
#include "common/log/log.h"

#include "projects_handler.h"

namespace server
{
namespace handlers
{

ProjectsHandler::ProjectsHandler(
    std::shared_ptr<services::IProjectService> projectService
)
    : m_projectService(std::move(projectService))
{
    if (!m_projectService)
    {
        LOG_WARN << "ProjectsHandler инициализирован без ProjectService";
    }
}

void ProjectsHandler::handleGetProjects(
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

    std::optional<int64_t> parentId = std::nullopt;
    if (params.count("parentId"))
        parentId = std::stoll(params["parentId"]);

    std::optional<bool> isArchive = std::nullopt;
    if (params.count("isArchive"))
        isArchive = params["isArchive"] == "true";

    std::string searchCaption = "";
    if (params.count("searchCaption"))
        searchCaption = params["searchCaption"];

    auto projectsPage = m_projectService->projects(
        page,
        pageSize,
        parentId,
        isArchive,
        searchCaption
    );

    web::json::value response;
    web::json::value items = web::json::value::array();

    for (size_t i = 0; i < projectsPage.projects.size(); ++i)
    {
        items[i] = dto::toWebJson(projectsPage.projects[i].toJson());
    }

    response["items"] = items;
    response["totalCount"] = web::json::value::number(projectsPage.totalCount);
    response["page"] = web::json::value::number(page);
    response["pageSize"] = web::json::value::number(pageSize);

    request.reply(web::http::status_codes::OK, response);
}

void ProjectsHandler::handleGetProject(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    int64_t id = extractProjectIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid project ID");
        request.reply(resp);
        return;
    }

    auto project = m_projectService->project(id);
    if (!project)
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Project not found");
        request.reply(resp);
        return;
    }

    request.reply(
        web::http::status_codes::OK,
        dto::toWebJson(project->toJson())
    );
}

void ProjectsHandler::handleCreateProject(
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
                    dto::Project project(nlohmannJson);

                    auto created = m_projectService->createProject(project);
                    if (!created)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::BadRequest
                        );
                        sendErrorResponse(
                            resp,
                            400,
                            "Invalid project data or could not create project"
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

void ProjectsHandler::handleUpdateProject(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractProjectIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid project ID");
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
                    nlohmannJson["id"] = id; // Гарантируем, что ID в пути и в теле совпадают
                    dto::Project project(nlohmannJson);

                    auto updated = m_projectService->updateProject(project);
                    if (!updated)
                    {
                        web::http::http_response resp(
                            web::http::status_codes::NotFound
                        );
                        sendErrorResponse(
                            resp,
                            404,
                            "Project not found or update failed"
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

void ProjectsHandler::handleDeleteProject(
    const web::http::http_request& request,
    const std::string& /*userId*/
)
{
    const int64_t id = extractProjectIdFromPath(request);
    if (id <= 0)
    {
        web::http::http_response resp(web::http::status_codes::BadRequest);
        sendErrorResponse(resp, 400, "Invalid project ID");
        request.reply(resp);
        return;
    }

    // Архивация вместо полного удаления
    if (m_projectService->archiveProject(id))
    {
        request.reply(web::http::status_codes::NoContent);
    }
    else
    {
        web::http::http_response resp(web::http::status_codes::NotFound);
        sendErrorResponse(resp, 404, "Project not found");
        request.reply(resp);
    }
}

int64_t ProjectsHandler::extractProjectIdFromPath(
    const web::http::http_request& request
)
{
    std::string path = web::uri::decode(request.relative_uri().path());
    std::regex pattern(R"(/api/projects/(\d+))");
    std::smatch matches;
    if (std::regex_match(path, matches, pattern) && matches.size() > 1)
    {
        return std::stoll(matches[1].str());
    }
    return -1;
}

std::map<std::string, std::string> ProjectsHandler::extractQueryParams(
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

void ProjectsHandler::sendErrorResponse(
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
