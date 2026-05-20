#include <cstdlib>

#include <nlohmann/json.hpp>

#include "common/log/log.h"

#include "application.h"
#include "config.h"

#include "api/middleware/impl/auth_middleware.h"
#include "api/rest_server.h"

#include "logic/impl/auth_service.h"
#include "logic/impl/edge_service.h"
#include "logic/impl/field_type_service.h"
#include "logic/impl/item_type_service.h"
#include "logic/impl/phase_service.h"
#include "logic/impl/project_service.h"
#include "logic/impl/state_service.h"
#include "logic/impl/user_service.h"
#include "logic/impl/workflow_service.h"

#include "repo/sqlite/sqlite_edge_repository.h"
#include "repo/sqlite/sqlite_field_type_repository.h"
#include "repo/sqlite/sqlite_item_type_repository.h"
#include "repo/sqlite/sqlite_phase_repository.h"
#include "repo/sqlite/sqlite_project_repository.h"
#include "repo/sqlite/sqlite_state_repository.h"
#include "repo/sqlite/sqlite_user_repository.h"
#include "repo/sqlite/sqlite_workflow_repository.h"

#include "storage/database_factory.h"

namespace server
{

Application::Application()
    : m_isRunning(false)
{
}

Application::~Application()
{
    cleanup();
}

bool Application::initialize()
{
    LOG_INFO << "Инициализация приложения...";

    // 1. Инициализируем базу данных
    try
    {
        // Создаем базу данных через фабрику
        m_database = db::DatabaseFactory::create(db::DatabaseType::Sqlite);

        db::DatabaseConfig dbConfig;
        dbConfig["database"] = CONFIG.database.file;
        m_database->initialize(dbConfig);

        LOG_INFO << "База данных инициализирована: " << CONFIG.database.file;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR << "Не удалось инициализировать базу данных: " << e.what();
        return false;
    }

    // 2. Создаем репозитории и сервисы
    auto userRepository = std::make_shared<repositories::SqliteUserRepository>(m_database);
    auto userService = std::make_shared<services::UserService>(userRepository);

    auto phaseRepository = std::make_shared<repositories::SqlitePhaseRepository>(m_database);
    auto phaseService = std::make_shared<services::PhaseService>(phaseRepository);

    auto projectRepository = std::make_shared<repositories::SqliteProjectRepository>(m_database);
    auto projectService = std::make_shared<services::ProjectService>(projectRepository);

    auto workflowRepository = std::make_shared<repositories::SqliteWorkflowRepository>(m_database);
    auto stateRepository = std::make_shared<repositories::SqliteStateRepository>(m_database);
    auto edgeRepository = std::make_shared<repositories::SqliteEdgeRepository>(m_database);

    auto workflowService = std::make_shared<services::WorkflowService>(
        workflowRepository, stateRepository, edgeRepository
    );
    auto stateService = std::make_shared<services::StateService>(
        stateRepository, edgeRepository, workflowRepository
    );
    auto edgeService = std::make_shared<services::EdgeService>(
        edgeRepository, stateRepository
    );

    // 3. Создаем middleware для аутентификации
    // TODO: Вынести секретный ключ в конфиг
    auto authMiddleware = std::make_shared<AuthMiddleware>(
        "your-very-long-secret-key-that-is-at-least-32-bytes-long!"
    );

    // 4. Создаем сервис аутентификации
    auto authService = std::make_shared<services::AuthService>(
        userRepository,
        authMiddleware
    );

    // 5. Создаем REST-сервер
    m_restServer = std::make_unique<RestServer>(
        CONFIG.network.apiHost,
        CONFIG.network.apiPort
    );

    m_restServer->setAuthMiddleware(authMiddleware);
    m_restServer->setAuthService(authService);
    m_restServer->setEdgeService(edgeService);
    m_restServer->setFieldTypeService(
        std::make_shared<services::FieldTypeService>(
            std::make_shared<repositories::SqliteFieldTypeRepository>(m_database)
        )
    );
    m_restServer->setItemTypeService(
        std::make_shared<services::ItemTypeService>(
            std::make_shared<repositories::SqliteItemTypeRepository>(m_database)
        )
    );
    m_restServer->setPhaseService(phaseService);
    m_restServer->setProjectService(projectService);
    m_restServer->setUserService(userService);
    m_restServer->setStateService(stateService);
    m_restServer->setWorkflowService(workflowService);

    if (!m_restServer->initialize())
    {
        LOG_ERROR << "Не удалось инициализировать REST-сервер";
        return false;
    }

    LOG_INFO << "Приложение успешно инициализировано";
    return true;
}

int Application::run()
{
    LOG_INFO << "Запуск приложения...";

    if (!initialize())
    {
        LOG_ERROR << "Не удалось инициализировать приложение";
        return EXIT_FAILURE;
    }

    m_isRunning = true;

    if (!m_restServer->start())
    {
        LOG_ERROR << "Не удалось запустить REST-сервер";
        return EXIT_FAILURE;
    }

    LOG_INFO
        << "Приложение запущено на "
        << CONFIG.network.apiHost << ":" << CONFIG.network.apiPort
        << ". Нажмите Ctrl+C для остановки.";

    while (m_isRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}

void Application::stop()
{
    LOG_INFO << "Остановка приложения...";
    m_isRunning = false;

    if (m_restServer)
    {
        m_restServer->stop();
    }

    cleanup();
    LOG_INFO << "Приложение остановлено";
}

void Application::cleanup()
{
    if (m_restServer)
    {
        m_restServer.reset();
    }

    if (m_database)
    {
        m_database->shutdown();
        m_database.reset();
    }
}

} // namespace server
