#pragma once

#include <atomic>
#include <memory>

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
class IUserService;
class IProjectService;
}

namespace repositories
{
class IProjectRepository;
class IUserRepository;
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

    std::shared_ptr<repositories::IProjectRepository> m_projectRepository;
    std::shared_ptr<repositories::IUserRepository> m_userRepository;

    std::shared_ptr<AuthMiddleware> m_authMiddleware;

    std::shared_ptr<services::AuthService> m_authService;
    std::shared_ptr<services::IProjectService> m_projectService;
    std::shared_ptr<services::IUserService> m_userService;

    std::atomic<bool> m_isRunning { false };
};

} // namespace server
