#pragma once

#include <memory>
#include <string>

#include <cpprest/http_msg.h>
#include <cpprest/json.h>

#include "logic/iphase_service.h"

namespace server
{
namespace handlers
{

/**
 * @brief Обработчик запросов для работы с фазами (CRUD операции).
 */
class PhasesHandler final
{
public:
    explicit PhasesHandler(std::shared_ptr<services::IPhaseService> phaseService);

    void handleGetPhases(
        const web::http::http_request& request,
        const std::string& userId
    );
    void handleGetPhase(
        const web::http::http_request& request,
        const std::string& userId
    );
    void handleCreatePhase(
        const web::http::http_request& request,
        const std::string& userId
    );
    void handleUpdatePhase(
        const web::http::http_request& request,
        const std::string& userId
    );
    void handleDeletePhase(
        const web::http::http_request& request,
        const std::string& userId
    );

private:
    int64_t extractPhaseIdFromPath(const web::http::http_request& request);

    std::map<std::string, std::string> extractQueryParams(
        const web::http::http_request& request
    );

    void sendErrorResponse(
        web::http::http_response& response,
        int code,
        const std::string& message
    );

private:
    std::shared_ptr<services::IPhaseService> m_phaseService;
};

} // namespace handlers
} // namespace server
