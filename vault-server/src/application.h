#pragma once

#include <memory>
#include <atomic>

namespace db
{
class IDatabase;
}

namespace server
{

class RestServer;
class AuthMiddleware;

namespace services
{
class AuthService;
class IEdgeService;
class IUserService;
class IStateService;
class IWorkflowService;
}

namespace repositories
{
class IEdgeRepository;
class IUserRepository;
class IStateRepository;
class IWorkflowRepository;
}

class Application final
{
public:
    explicit Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    int run();
    void stop();

private:
    bool initialize();
    void cleanup();

private:
    std::unique_ptr<RestServer> m_restServer;
    std::shared_ptr<db::IDatabase> m_database;
    std::shared_ptr<repositories::IEdgeRepository> m_edgeRepository;
    std::shared_ptr<repositories::IUserRepository> m_userRepository;
    std::shared_ptr<repositories::IStateRepository> m_stateRepository;
    std::shared_ptr<repositories::IWorkflowRepository> m_workflowRepository;
    std::shared_ptr<AuthMiddleware> m_authMiddleware;
    std::shared_ptr<services::AuthService> m_authService;
    std::shared_ptr<services::IEdgeService> m_edgeService;
    std::shared_ptr<services::IUserService> m_userService;
    std::shared_ptr<services::IStateService> m_stateService;
    std::shared_ptr<services::IWorkflowService> m_workflowService;

    std::atomic<bool> m_isRunning { false };
};

} // namespace server
