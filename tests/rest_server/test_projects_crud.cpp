#include <chrono>
#include <thread>

#include <boost/test/unit_test.hpp>

#include <cpprest/http_client.h>

#include "api/rest_server.h"

#include "tests/server_mocks/mock_auth_middleware.h"
#include "tests/server_mocks/mock_auth_service.h"
#include "tests/server_mocks/mock_project_service.h"
#include "tests/server_mocks/mock_user_service.h"

using namespace web;
using namespace web::http;

namespace server
{
namespace tests
{

struct ProjectsTestFixture
{
    ProjectsTestFixture()
    {
        mockAuthMiddleware = std::make_shared<MockAuthMiddleware>();
        mockAuthService = std::make_shared<MockAuthService>();
        mockUserService = std::make_shared<MockUserService>();
        mockProjectService = std::make_shared<MockProjectService>();

        mockAuthMiddleware->setValidateRequestResult(true, "test_user_123");

        // Настройка тестовых данных по умолчанию
        setupDefaultProjectService();

        server = std::make_unique<RestServer>("127.0.0.1", 18084);
        server->setAuthMiddleware(mockAuthMiddleware);
        server->setAuthService(mockAuthService);
        server->setUserService(mockUserService);
        server->setProjectService(mockProjectService);

        BOOST_REQUIRE(server->initialize());

        serverThread = std::thread(
            [this]()
            {
                server->start();
            }
        );
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void setupDefaultProjectService()
    {
        // Настройка списка проектов
        services::ProjectsPage testPage;
        dto::Project project1;
        project1.id = 1;
        project1.caption = "Проект 1";
        project1.description = "Описание 1";
        project1.isArchive = false;

        dto::Project project2;
        project2.id = 2;
        project2.caption = "Проект 2";
        project2.description = "Описание 2";
        project2.parentId = 1;
        project2.isArchive = false;

        testPage.projects = { project1, project2 };
        testPage.totalCount = 2;
        mockProjectService->setGetProjectsResult(testPage);
        mockProjectService->setGetProjectResult(project1);

        dto::Project newProject;
        newProject.id = 3;
        newProject.caption = "Новый проект";
        newProject.description = "Описание";
        newProject.isArchive = false;
        mockProjectService->setCreateProjectResult(newProject);

        mockProjectService->setUpdateProjectResult(project1);
        mockProjectService->setArchiveProjectResult(true);
        mockProjectService->setRestoreProjectResult(true);
    }

    ~ProjectsTestFixture()
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
        web::http::client::http_client client(U("http://127.0.0.1:18084"));
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
        web::http::client::http_client client(U("http://127.0.0.1:18084"));
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
        web::http::client::http_client client(U("http://127.0.0.1:18084"));
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
        web::http::client::http_client client(U("http://127.0.0.1:18084"));
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
    std::shared_ptr<MockProjectService> mockProjectService;
    std::thread serverThread;
};

BOOST_FIXTURE_TEST_SUITE(ProjectsCrudTestSuite, ProjectsTestFixture)

// ============================================================
// GET /api/projects — Получение списка проектов
// ============================================================

BOOST_AUTO_TEST_CASE(test_get_projects_returns_list)
{
    auto response = makeGetRequest("/api/projects").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockProjectService->getProjectsCallCount(), 1);

    auto json = response.extract_json().get();
    BOOST_CHECK(json.has_field(U("items")));
    BOOST_CHECK(json.has_field(U("totalCount")));
    BOOST_CHECK_EQUAL(json.at(U("totalCount")).as_integer(), 2);
    BOOST_CHECK_EQUAL(json.at(U("items")).as_array().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_get_projects_with_pagination_params)
{
    services::ProjectsPage emptyPage;
    mockProjectService->setGetProjectsResult(emptyPage);

    auto response = makeGetRequest("/api/projects?page=3&pageSize=5").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockProjectService->getLastGetProjectsPage(), 3);
    BOOST_CHECK_EQUAL(mockProjectService->getLastGetProjectsPageSize(), 5);
}

BOOST_AUTO_TEST_CASE(test_get_projects_empty_list)
{
    services::ProjectsPage emptyPage;
    mockProjectService->setGetProjectsResult(emptyPage);

    auto response = makeGetRequest("/api/projects").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    auto json = response.extract_json().get();
    BOOST_CHECK_EQUAL(json.at(U("items")).as_array().size(), 0);
    BOOST_CHECK_EQUAL(json.at(U("totalCount")).as_integer(), 0);
}

BOOST_AUTO_TEST_CASE(test_get_projects_requires_auth)
{
    auto response = makeGetRequest("/api/projects", "").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::Unauthorized);
    BOOST_CHECK_EQUAL(mockProjectService->getProjectsCallCount(), 0);
}

// ============================================================
// GET /api/projects/{id} — Получение проекта по ID
// ============================================================

BOOST_AUTO_TEST_CASE(test_get_project_by_id_success)
{
    dto::Project project;
    project.id = 42;
    project.caption = "Конкретный проект";
    mockProjectService->setGetProjectResult(project);

    auto response = makeGetRequest("/api/projects/42").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockProjectService->getProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(mockProjectService->getLastGetProjectId(), 42);

    auto json = response.extract_json().get();
    BOOST_CHECK_EQUAL(json.at(U("id")).as_integer(), 42);
    BOOST_CHECK_EQUAL(json.at(U("caption")).as_string(), U("Конкретный проект"));
}

BOOST_AUTO_TEST_CASE(test_get_project_not_found)
{
    mockProjectService->setGetProjectResult(std::nullopt);

    auto response = makeGetRequest("/api/projects/999").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockProjectService->getProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(mockProjectService->getLastGetProjectId(), 999);
}

// ============================================================
// POST /api/projects — Создание проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_create_project_success)
{
    dto::Project createdProject;
    createdProject.id = 100;
    createdProject.caption = "Новый проект";
    createdProject.description = "Описание нового проекта";
    mockProjectService->setCreateProjectResult(createdProject);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Новый проект"));
    body[U("description")] = web::json::value::string(U("Описание нового проекта"));

    auto response = makePostRequest("/api/projects", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::Created);
    BOOST_CHECK_EQUAL(mockProjectService->createProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(*mockProjectService->getLastCreatedProject().caption, "Новый проект");

    auto json = response.extract_json().get();
    BOOST_CHECK_EQUAL(json.at(U("id")).as_integer(), 100);
    BOOST_CHECK_EQUAL(json.at(U("caption")).as_string(), U("Новый проект"));
}

BOOST_AUTO_TEST_CASE(test_create_project_missing_required_fields)
{
    web::json::value body;
    body[U("description")] = web::json::value::string(U("Проект без названия"));

    auto response = makePostRequest("/api/projects", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::BadRequest);
    BOOST_CHECK_EQUAL(mockProjectService->createProjectCallCount(), 0);
}

// ============================================================
// PUT /api/projects/{id} — Обновление проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_update_project_success)
{
    dto::Project updatedProject;
    updatedProject.id = 1;
    updatedProject.caption = "Обновленный проект";
    mockProjectService->setUpdateProjectResult(updatedProject);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Обновленный проект"));

    auto response = makePutRequest("/api/projects/1", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    BOOST_CHECK_EQUAL(mockProjectService->updateProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(*mockProjectService->getLastUpdatedProject().id, 1);
    BOOST_CHECK_EQUAL(*mockProjectService->getLastUpdatedProject().caption, "Обновленный проект");
}

BOOST_AUTO_TEST_CASE(test_update_project_not_found)
{
    mockProjectService->setUpdateProjectResult(std::nullopt);

    web::json::value body;
    body[U("caption")] = web::json::value::string(U("Несуществующий"));

    auto response = makePutRequest("/api/projects/999", body).get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockProjectService->updateProjectCallCount(), 1);
}

// ============================================================
// DELETE /api/projects/{id} — Архивирование проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_archive_project_success)
{
    mockProjectService->setArchiveProjectResult(true);

    auto response = makeDeleteRequest("/api/projects/2").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NoContent);
    BOOST_CHECK_EQUAL(mockProjectService->archiveProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(mockProjectService->getLastArchivedProjectId(), 2);
}

BOOST_AUTO_TEST_CASE(test_archive_project_not_found)
{
    mockProjectService->setArchiveProjectResult(false);

    auto response = makeDeleteRequest("/api/projects/999").get();

    BOOST_CHECK_EQUAL(response.status_code(), status_codes::NotFound);
    BOOST_CHECK_EQUAL(mockProjectService->archiveProjectCallCount(), 1);
    BOOST_CHECK_EQUAL(mockProjectService->getLastArchivedProjectId(), 999);
}

// ============================================================
// Интеграционный тест (полный цикл)
// ============================================================

BOOST_AUTO_TEST_CASE(test_full_project_lifecycle)
{
    // 1. Создание
    dto::Project newProject;
    newProject.id = 100;
    newProject.caption = "Жизненный цикл";
    mockProjectService->setCreateProjectResult(newProject);

    web::json::value createBody;
    createBody[U("caption")] = web::json::value::string(U("Жизненный цикл"));

    auto createResponse = makePostRequest("/api/projects", createBody).get();
    BOOST_CHECK_EQUAL(createResponse.status_code(), status_codes::Created);

    // 2. Чтение
    mockProjectService->setGetProjectResult(newProject);
    auto getResponse = makeGetRequest("/api/projects/100").get();
    BOOST_CHECK_EQUAL(getResponse.status_code(), status_codes::OK);

    // 3. Обновление
    dto::Project updatedProject = newProject;
    updatedProject.caption = "Обновленный цикл";
    mockProjectService->setUpdateProjectResult(updatedProject);

    web::json::value updateBody;
    updateBody[U("caption")] = web::json::value::string(U("Обновленный цикл"));

    auto updateResponse = makePutRequest("/api/projects/100", updateBody).get();
    BOOST_CHECK_EQUAL(updateResponse.status_code(), status_codes::OK);

    // 4. Архивирование
    mockProjectService->setArchiveProjectResult(true);
    auto archiveResponse = makeDeleteRequest("/api/projects/100").get();
    BOOST_CHECK_EQUAL(archiveResponse.status_code(), status_codes::NoContent);

    // 5. Проверка после архивации
    dto::Project archivedProject = updatedProject;
    archivedProject.isArchive = true;
    mockProjectService->setGetProjectResult(archivedProject);
    auto getArchived = makeGetRequest("/api/projects/100").get();
    BOOST_CHECK_EQUAL(getArchived.status_code(), status_codes::OK);

    auto archivedJson = getArchived.extract_json().get();
    BOOST_CHECK(archivedJson.has_field(U("isArchive")));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace server
