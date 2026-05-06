#include <chrono>
#include <thread>

#include <boost/test/unit_test.hpp>

#include <cpprest/http_client.h>

#include "api/rest_server.h"

#include "tests/server_mocks/mock_auth_middleware.h"
#include "tests/server_mocks/mock_auth_service.h"
#include "tests/server_mocks/mock_phase_service.h"
#include "tests/server_mocks/mock_user_service.h"

using namespace web;
using namespace web::http;

namespace server
{
namespace tests
{

struct PhasesTestFixture
{
    PhasesTestFixture()
    {
        mockAuthMiddleware = std::make_shared<MockAuthMiddleware>();
        mockAuthService = std::make_shared<MockAuthService>();
        mockUserService = std::make_shared<MockUserService>();
        mockPhaseService = std::make_shared<MockPhaseService>();

        mockAuthMiddleware->setValidateRequestResult(true, "test_user_123");

        // Настройка тестовых данных по умолчанию
        setupDefaultPhaseService();

        server = std::make_unique<RestServer>("127.0.0.1", 18085);
        server->setAuthMiddleware(mockAuthMiddleware);
        server->setAuthService(mockAuthService);
        server->setUserService(mockUserService);
        server->setPhaseService(mockPhaseService);

        BOOST_REQUIRE(server->initialize());

        serverThread = std::thread(
            [this]()
            {
                server->start();
            }
        );
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void setupDefaultPhaseService()
    {
        services::PhasesPage testPage;
        dto::Phase phase1;
        phase1.id = 1;
        phase1.caption = "Фаза 1";
        phase1.description = "Описание фазы 1";
        phase1.projectId = 10;

        dto::Phase phase2;
        phase2.id = 2;
        phase2.caption = "Фаза 2";
        phase2.description = "Описание фазы 2";
        phase2.projectId = 10;

        testPage.phases = { phase1, phase2 };
        testPage.totalCount = 2;
        mockPhaseService->setGetPhasesResult(testPage);
        mockPhaseService->setGetPhaseResult(phase1);

        dto::Phase newPhase;
        newPhase.id = 3;
        newPhase.caption = "Новая фаза";
        newPhase.projectId = 10;
        mockPhaseService->setCreatePhaseResult(newPhase);

        mockPhaseService->setUpdatePhaseResult(phase1);
        mockPhaseService->setArchivePhaseResult(true);
        mockPhaseService->setRestorePhaseResult(true);
    }

    ~PhasesTestFixture()
    {
        if (server)
            server->stop();
        if (serverThread.joinable())
            serverThread.join();
    }

    pplx::task<web::http::http_response> makeGetRequest(
        const std::string& path,
        const std::string& token = "valid_token"
    )
    {
        web::http::client::http_client client(U("http://127.0.0.1:18085"));
        web::http::http_request request(methods::GET);
        request.set_request_uri(U(path));
        if (!token.empty())
        {
            request.headers().add(U("Authorization"), U("Bearer " + token));
        }
        return client.request(request);
    }

    pplx::task<web::http::http_response> makePostRequest(
        const std::string& path,
        const web::json::value& body,
        const std::string& token = "valid_token"
    )
    {
        web::http::client::http_client client(U("http://127.0.0.1:18085"));
        web::http::http_request request(methods::POST);
        request.set_request_uri(U(path));
        if (!token.empty())
        {
            request.headers().add(U("Authorization"), U("Bearer " + token));
        }
        request.set_body(body);
        return client.request(request);
    }

    pplx::task<web::http::http_response> makePutRequest(
        const std::string& path,
        const web::json::value& body,
        const std::string& token = "valid_token"
    )
    {
        web::http::client::http_client client(U("http://127.0.0.1:18085"));
        web::http::http_request request(methods::PUT);
        request.set_request_uri(U(path));
        if (!token.empty())
        {
            request.headers().add(U("Authorization"), U("Bearer " + token));
        }
        request.set_body(body);
        return client.request(request);
    }

    pplx::task<web::http::http_response> makeDeleteRequest(
        const std::string& path,
        const std::string& token = "valid_token"
    )
    {
        web::http::client::http_client client(U("http://127.0.0.1:18085"));
        web::http::http_request request(methods::DEL);
        request.set_request_uri(U(path));
        if (!token.empty())
        {
            request.headers().add(U("Authorization"), U("Bearer " + token));
        }
        return client.request(request);
    }

    std::unique_ptr<RestServer> server;
    std::shared_ptr<MockAuthMiddleware> mockAuthMiddleware;
    std::shared_ptr<MockAuthService> mockAuthService;
    std::shared_ptr<MockUserService> mockUserService;
    std::shared_ptr<MockPhaseService> mockPhaseService;
    std::thread serverThread;
};

BOOST_FIXTURE_TEST_SUITE(PhasesCrudTestSuite, PhasesTestFixture)

// ============================================================
// GET /api/phases — Получение списка фаз
// ============================================================

BOOST_AUTO_TEST_CASE(test_get_phases_returns_list)
{
    auto response = makeGetRequest("/api/phases").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockPhaseService->getPhasesCallCount(), 1);

    auto json = response.extract_json().get();
    BOOST_CHECK(json.has_field(U("items")));
    BOOST_CHECK(json.has_field(U("totalCount")));
    BOOST_CHECK_EQUAL(json.at(U("totalCount")).as_integer(), 2);
    BOOST_CHECK_EQUAL(json.at(U("items")).as_array().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_get_phases_filtered_by_project)
{
    services::PhasesPage emptyPage;
    mockPhaseService->setGetPhasesResult(emptyPage);

    auto response = makeGetRequest("/api/phases?projectId=10").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK(mockPhaseService->getLastGetPhasesProjectId().has_value());
    BOOST_CHECK_EQUAL(mockPhaseService->getLastGetPhasesProjectId().value(), 10);
}

// ============================================================
// GET /api/phases/{id} — Получение фазы по ID
// ============================================================

BOOST_AUTO_TEST_CASE(test_get_phase_by_id_success)
{
    dto::Phase phase;
    phase.id = 42;
    phase.caption = "Конкретная фаза";
    mockPhaseService->setGetPhaseResult(phase);

    auto response = makeGetRequest("/api/phases/42").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockPhaseService->getPhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(mockPhaseService->getLastGetPhaseId(), 42);

    auto json = response.extract_json().get();
    BOOST_CHECK_EQUAL(json.at(U("id")).as_integer(), 42);
    BOOST_CHECK_EQUAL(json.at(U("caption")).as_string(), U("Конкретная фаза"));
}

BOOST_AUTO_TEST_CASE(test_get_phase_not_found)
{
    mockPhaseService->setGetPhaseResult(std::nullopt);

    auto response = makeGetRequest("/api/phases/999").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockPhaseService->getPhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(mockPhaseService->getLastGetPhaseId(), 999);
}

// ============================================================
// POST /api/phases — Создание фазы
// ============================================================

BOOST_AUTO_TEST_CASE(test_create_phase_success)
{
    dto::Phase createdPhase;
    createdPhase.id = 100;
    createdPhase.caption = "Новая фаза";
    createdPhase.projectId = 10;
    mockPhaseService->setCreatePhaseResult(createdPhase);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Новая фаза"));
    body[U("projectId")] = web::json::value::number(10);

    auto response = makePostRequest("/api/phases", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::Created);
    BOOST_CHECK_EQUAL(mockPhaseService->createPhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(*mockPhaseService->getLastCreatedPhase().caption, "Новая фаза");
    BOOST_CHECK_EQUAL(*mockPhaseService->getLastCreatedPhase().projectId, 10);

    auto json = response.extract_json().get();
    BOOST_CHECK_EQUAL(json.at(U("id")).as_integer(), 100);
    BOOST_CHECK_EQUAL(json.at(U("caption")).as_string(), U("Новая фаза"));
}

BOOST_AUTO_TEST_CASE(test_create_phase_missing_caption)
{
    web::json::value body;
    body[U("projectId")] = web::json::value::number(10);

    auto response = makePostRequest("/api/phases", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::BadRequest);
    BOOST_CHECK_EQUAL(mockPhaseService->createPhaseCallCount(), 0);
}

// ============================================================
// PUT /api/phases/{id} — Обновление фазы
// ============================================================

BOOST_AUTO_TEST_CASE(test_update_phase_success)
{
    dto::Phase updatedPhase;
    updatedPhase.id = 1;
    updatedPhase.caption = "Обновленная фаза";
    mockPhaseService->setUpdatePhaseResult(updatedPhase);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Обновленная фаза"));

    auto response = makePutRequest("/api/phases/1", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockPhaseService->updatePhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(*mockPhaseService->getLastUpdatedPhase().id, 1);
}

BOOST_AUTO_TEST_CASE(test_update_phase_not_found)
{
    mockPhaseService->setUpdatePhaseResult(std::nullopt);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Несуществующая"));

    auto response = makePutRequest("/api/phases/999", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockPhaseService->updatePhaseCallCount(), 1);
}

// ============================================================
// DELETE /api/phases/{id} — Архивирование фазы
// ============================================================

BOOST_AUTO_TEST_CASE(test_archive_phase_success)
{
    mockPhaseService->setArchivePhaseResult(true);

    auto response = makeDeleteRequest("/api/phases/3").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NoContent);
    BOOST_CHECK_EQUAL(mockPhaseService->archivePhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(mockPhaseService->getLastArchivedPhaseId(), 3);
}

BOOST_AUTO_TEST_CASE(test_archive_phase_not_found)
{
    mockPhaseService->setArchivePhaseResult(false);

    auto response = makeDeleteRequest("/api/phases/999").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockPhaseService->archivePhaseCallCount(), 1);
    BOOST_CHECK_EQUAL(mockPhaseService->getLastArchivedPhaseId(), 999);
}

// ============================================================
// Интеграционный тест (полный цикл фазы)
// ============================================================

BOOST_AUTO_TEST_CASE(test_full_phase_lifecycle)
{
    // 1. Создание
    dto::Phase newPhase;
    newPhase.id = 100;
    newPhase.caption = "Жизненный цикл фазы";
    newPhase.projectId = 10;
    mockPhaseService->setCreatePhaseResult(newPhase);

    web::json::value createBody;
    createBody[U("caption")] = web::json::value::string(U("Жизненный цикл фазы"));
    createBody[U("projectId")] = web::json::value::number(10);

    auto createResponse = makePostRequest("/api/phases", createBody).get();
    BOOST_CHECK_EQUAL(createResponse.status_code(), status_codes::Created);

    // 2. Чтение
    mockPhaseService->setGetPhaseResult(newPhase);
    auto getResponse = makeGetRequest("/api/phases/100").get();
    BOOST_CHECK_EQUAL(getResponse.status_code(), status_codes::OK);

    // 3. Обновление
    dto::Phase updatedPhase = newPhase;
    updatedPhase.caption = "Обновленная фаза";
    mockPhaseService->setUpdatePhaseResult(updatedPhase);

    web::json::value updateBody;
    updateBody[U("caption")] = web::json::value::string(U("Обновленная фаза"));

    auto updateResponse = makePutRequest("/api/phases/100", updateBody).get();
    BOOST_CHECK_EQUAL(updateResponse.status_code(), status_codes::OK);

    // 4. Архивирование
    mockPhaseService->setArchivePhaseResult(true);
    auto archiveResponse = makeDeleteRequest("/api/phases/100").get();
    BOOST_CHECK_EQUAL(archiveResponse.status_code(), status_codes::NoContent);

    // 5. Проверка в архиве
    dto::Phase archivedPhase = updatedPhase;
    archivedPhase.isArchive = true;
    mockPhaseService->setGetPhaseResult(archivedPhase);
    auto getArchived = makeGetRequest("/api/phases/100").get();
    BOOST_CHECK_EQUAL(getArchived.status_code(), status_codes::OK);

    auto archivedJson = getArchived.extract_json().get();
    BOOST_CHECK(archivedJson.has_field(U("isArchive")));
    BOOST_CHECK_EQUAL(archivedJson.at(U("caption")).as_string(), U("Обновленная фаза"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace server
