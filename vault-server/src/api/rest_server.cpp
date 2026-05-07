#include <chrono>
#include <memory>
#include <regex>

#include <cpprest/http_msg.h>
#include <cpprest/uri.h>

#include "common/log/log.h"

#include "api/handlers/auth_handler.h"
#include "api/handlers/items_handler.h"
#include "api/handlers/phases_handler.h"
#include "api/handlers/projects_handler.h"
#include "api/handlers/users_handler.h"

#include "logic/iauth_service.h"
#include "logic/iproject_service.h"
#include "logic/iuser_service.h"

#include "rest_server.h"

namespace server
{

RestServer::RestServer(const std::string& host, uint16_t port)
    : m_host(host)
    , m_port(port)
    , m_isRunning(false)
{
    LOG_DEBUG
        << "REST-сервера создан. Хост=" << m_host << ", порт=" << m_port << ".";
}

RestServer::~RestServer()
{
    stop(); // Гарантируем остановку сервера при разрушении объекта
}

bool RestServer::initialize()
{
    LOG_DEBUG << "Инициализация REST-сервера...";

    registerRoutes(); // Регистрируем все маршруты API
    setupListener(); // Настраиваем HTTP-слушатель

    LOG_DEBUG << "REST-сервера успешно инициализирован";
    return true;
}

bool RestServer::start()
{
    if (m_isRunning)
    {
        LOG_ERROR << "Сервер REST уже запущен";
        return false;
    }

    try
    {
        m_listener->open().wait(); // Открываем слушатель и ждем готовности
        m_isRunning = true;
        LOG_INFO
            << "Сервер REST успешно запущен на "
            << m_host << ":" << m_port;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Не удалось запустить сервер: " << e.what();
        return false;
    }

    return true;
}

void RestServer::stop()
{
    if (!m_isRunning)
    {
        return;
    }

    LOG_DEBUG << "Остановка REST-сервера...";
    m_isRunning = false;

    try
    {
        if (m_listener)
        {
            m_listener->close().wait(); // Закрываем слушатель и ждем завершения
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Ошибка при остановке REST-сервера: " << e.what();
    }

    LOG_INFO << "Сервер REST остановлен";
}

void RestServer::setAuthMiddleware(std::shared_ptr<IAuthMiddleware> middleware)
{
    m_authMiddleware = middleware;
}

void RestServer::setAuthService(std::shared_ptr<services::IAuthService> authService)
{
    m_authService = authService;
}

void RestServer::setPhaseService(std::shared_ptr<services::IPhaseService> phaseService)
{
    m_phaseService = phaseService;
}

void RestServer::setProjectService(std::shared_ptr<services::IProjectService> projectService)
{
    m_projectService = projectService;
}

void RestServer::setUserService(std::shared_ptr<services::IUserService> userService)
{
    m_userService = userService;
}

void RestServer::registerRoutes()
{
    // ===== Аутентификация =====
    if (m_authService)
    {
        auto authHandler = std::make_shared<handlers::AuthHandler>(m_authService);

        addRoutePost(
            "/auth/login",
            [authHandler](const auto& request, const auto& /*userId*/)
            {
                authHandler->handleLogin(request);
            },
            true
        );
        addRoutePost(
            "/auth/logout",
            [authHandler](const auto& request, const auto& /*userId*/)
            {
                authHandler->handleLogout(request);
            },
            true
        );
        addRoutePost(
            "/auth/change-password",
            [authHandler](const auto& request, const auto& userId)
            {
                authHandler->handleChangePassword(request, userId);
            },
            false // требуется аутентификация
        );
    }

    // ===== Элементы =====
    {
        auto itemsHandler = std::make_shared<handlers::ItemsHandler>();

        addRouteGet(
            "/api/items",
            [itemsHandler](const auto& request, const auto& userId)
            {
                itemsHandler->handleGetItems(request, userId);
            }
        );
        addRoutePost(
            "/api/items",
            [itemsHandler](const auto& request, const auto& userId)
            {
                itemsHandler->handleCreateItem(request, userId);
            }
        );
        addRouteGet(
            R"(/api/items/(\d+))",
            [itemsHandler](const auto& request, const auto& userId)
            {
                itemsHandler->handleGetItem(request, userId);
            },
            false
        );
        addRoutePut(
            R"(/api/items/(\d+))",
            [itemsHandler](const auto& request, const auto& userId)
            {
                itemsHandler->handleUpdateItem(request, userId);
            },
            false
        );
        addRouteDel(
            R"(/api/items/(\d+))",
            [itemsHandler](const auto& request, auto& userId)
            {
                itemsHandler->handleDeleteItem(request, userId);
            },
            false
        );
    }

    // ===== Пользователи =====
    if (m_userService)
    {
        auto usersHandler = std::make_shared<handlers::UsersHandler>(m_userService);

        addRouteGet(
            "/api/users",
            [usersHandler](const auto& request, auto& userId)
            {
                usersHandler->handleGetUsers(request, userId);
            }
        );
        addRoutePost(
            "/api/users",
            [usersHandler](const auto& request, auto& userId)
            {
                usersHandler->handleCreateUser(request, userId);
            }
        );
        addRouteGet(
            R"(/api/users/(\d+))",
            [usersHandler](const auto& request, auto& userId)
            {
                usersHandler->handleGetUser(request, userId);
            }
        );
        addRoutePut(
            R"(/api/users/(\d+))",
            [usersHandler](const auto& request, auto& userId)
            {
                usersHandler->handleUpdateUser(request, userId);
            }
        );
        addRouteDel(
            R"(/api/users/(\d+))",
            [usersHandler](const auto& request, auto& userId)
            {
                usersHandler->handleDeleteUser(request, userId);
            }
        );
    }

    // ===== Фазы =====
    if (m_phaseService)
    {
        auto phasesHandler = std::make_shared<handlers::PhasesHandler>(m_phaseService);

        addRouteGet(
            "/api/phases",
            [phasesHandler](const auto& request, const auto& userId)
            {
                phasesHandler->handleGetPhases(request, userId);
            }
        );
        addRoutePost(
            "/api/phases",
            [phasesHandler](const auto& request, const auto& userId)
            {
                phasesHandler->handleCreatePhase(request, userId);
            }
        );
        addRouteGet(
            R"(/api/phases/(\d+))",
            [phasesHandler](const auto& request, const auto& userId)
            {
                phasesHandler->handleGetPhase(request, userId);
            }
        );
        addRoutePut(
            R"(/api/phases/(\d+))",
            [phasesHandler](const auto& request, const auto& userId)
            {
                phasesHandler->handleUpdatePhase(request, userId);
            }
        );
        addRouteDel(
            R"(/api/phases/(\d+))",
            [phasesHandler](const auto& request, const auto& userId)
            {
                phasesHandler->handleDeletePhase(request, userId);
            }
        );
    }

    // ===== Проекты =====
    if (m_projectService)
    {
        auto projectsHandler = std::make_shared<handlers::ProjectsHandler>(m_projectService);

        addRouteGet(
            "/api/projects",
            [projectsHandler](const auto& request, const auto& userId)
            {
                projectsHandler->handleGetProjects(request, userId);
            }
        );

        addRoutePost(
            "/api/projects",
            [projectsHandler](const auto& request, const auto& userId)
            {
                projectsHandler->handleCreateProject(request, userId);
            }
        );

        addRouteGet(
            R"(/api/projects/(\d+))",
            [projectsHandler](const auto& request, const auto& userId)
            {
                projectsHandler->handleGetProject(request, userId);
            }
        );

        addRoutePut(
            R"(/api/projects/(\d+))",
            [projectsHandler](const auto& request, const auto& userId)
            {
                projectsHandler->handleUpdateProject(request, userId);
            }
        );

        addRouteDel(
            R"(/api/projects/(\d+))",
            [projectsHandler](const auto& request, const auto& userId)
            {
                projectsHandler->handleDeleteProject(request, userId);
            }
        );
    }

    // ===== Работоспособность сервера (Health check) =====
    {
        addRouteGet(
            "/health",
            [](const auto& request, const auto& /*userId*/)
            {
                const auto now = std::chrono::system_clock::now().time_since_epoch();
                web::json::value response;
                response["status"] = web::json::value::string("ok");
                response["timestamp"] = web::json::value::number(
                    std::chrono::duration_cast<std::chrono::seconds>(now).count()
                );
                request.reply(web::http::status_codes::OK, response);
            },
            true // публичный эндпоинт
        );
    }
    LOG_DEBUG
        << "Успешно зарегистрированные маршруты, всего: " << m_routes.size();
}

void RestServer::setupListener()
{
    const web::http::uri address = web::http::uri(
        "http://" + m_host + ":" + std::to_string(m_port)
    );
    m_listener = std::make_unique<web::http::experimental::listener::http_listener>(
        address
    );

    // Устанавливаем универсальный обработчик всех запросов
    m_listener->support(
        [this](web::http::http_request request)
        {
            try
            {
                handleRequest(std::move(request));
            }
            catch (const std::exception& e)
            {
                // Глобальный обработчик ошибок — любое неперехваченное исключение
                // приводит к ответу 500 Internal Server Error
                LOG_ERROR << "Ошибка обработки запроса: " << e.what();
                web::json::value error;
                error["code"] = web::json::value::number(500);
                error["message"] = web::json::value::string(
                    "Internal server error: " + std::string(e.what())
                );
                request.reply(web::http::status_codes::InternalError, error);
            }
        }
    );
}

void RestServer::handleRequest(web::http::http_request request)
{
    const std::string path = web::uri::decode(request.relative_uri().path());
    const auto method = request.method();

    LOG_INFO << "Входящий запрос: " << method << " " << path;

    RouteHandler handler;
    bool isPublic = false;
    std::map<std::string, std::string> pathParams;

    // Ищем обработчик для данного маршрута
    if (!matchRoute(method, path, handler, isPublic, pathParams))
    {
        // Маршрут не найден — возвращаем 404
        web::json::value error;
        error["code"] = web::json::value::number(404);
        error["message"] = web::json::value::string("Not found");
        request.reply(web::http::status_codes::NotFound, error);
        return;
    }

    // Применяем аутентификацию только для защищенных маршрутов
    if (!isPublic)
    {
        std::string userId;
        if (!applyAuthMiddleware(request, userId))
        {
            return; // Ответ уже отправлен в applyAuthMiddleware
        }
        handler(request, userId); // Вызываем обработчик с ID пользователя
    }
    else
    {
        handler(request, ""); // Публичный маршрут — userId не требуется
    }
}

bool RestServer::applyAuthMiddleware(
    const web::http::http_request& request,
    std::string& userId
)
{
    // Проверяем наличие middleware
    if (!m_authMiddleware)
    {
        LOG_ERROR << "Auth middleware не установлено";
        web::json::value error;
        error["code"] = web::json::value::number(500);
        error["message"] = web::json::value::string(
            "Internal server error: auth middleware not configured"
        );
        request.reply(web::http::status_codes::InternalError, error);
        return false;
    }

    // Ищем заголовок Authorization
    auto authHeader = request.headers().find("Authorization");
    if (authHeader == request.headers().end())
    {
        web::json::value error;
        error["code"] = web::json::value::number(401);
        error["message"] = web::json::value::string(
            "Missing Authorization header"
        );
        request.reply(web::http::status_codes::Unauthorized, error);
        return false;
    }

    // Валидируем токен
    if (!m_authMiddleware->validateRequest(authHeader->second, userId))
    {
        web::json::value error;
        error["code"] = web::json::value::number(401);
        error["message"] = web::json::value::string(
            "Invalid or expired token"
        );
        request.reply(web::http::status_codes::Unauthorized, error);
        return false;
    }

    return true; // Аутентификация успешна
}

void RestServer::addRoute(
    const web::http::method& method,
    const std::string& path,
    RouteHandler handler,
    bool isPublic
)
{
    RouteInfo route;
    route.method = method;
    route.pathPattern = path;
    route.handler = handler;
    route.isPublic = isPublic;
    m_routes.push_back(route);
}

bool RestServer::matchRoute(
    const web::http::method& method,
    const std::string& uriPath,
    RouteHandler& handler,
    bool& isPublic,
    std::map<std::string, std::string>& params
)
{
    for (const auto& route : m_routes)
    {
        // Проверяем соответствие HTTP-метода
        if (route.method != method)
            continue;

        // Проверяем соответствие пути с помощью регулярного выражения
        std::regex pattern(route.pathPattern);
        std::smatch matches;

        if (std::regex_match(uriPath, matches, pattern))
        {
            handler = route.handler;
            isPublic = route.isPublic;

            // Извлекаем параметры из пути (например, ID из /api/items/123)
            if (matches.size() > 1)
            {
                params["id"] = matches[1].str();
            }
            return true;
        }
    }
    return false; // Маршрут не найден
}

} // namespace server
