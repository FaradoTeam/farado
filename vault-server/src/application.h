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
class IPhaseService;
class IProjectService;
class IUserService;
}

namespace repositories
{
class IPhaseRepository;
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

    std::atomic<bool> m_isRunning { false };
};

} // namespace server
