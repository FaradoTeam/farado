#include <cstdio>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "common/dto/edge.h"

#include "repo/sqlite/sqlite_edge_repository.h"
#include "storage/database_factory.h"
#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct EdgeRepositoryFixture
{
    EdgeRepositoryFixture()
    {
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_edge_repo.db";
        std::remove(m_tempDbPath.c_str());

        db::DatabaseConfig config;
        config["database"] = m_tempDbPath.string();

        m_database = std::make_shared<db::SqliteDatabase>();
        m_database->initialize(config);

        auto conn = m_database->connection();

        // Создаем схему
        conn->execute(R"(
            CREATE TABLE Workflow (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                caption TEXT NOT NULL,
                description TEXT
            )
        )");

        conn->execute(R"(
            CREATE TABLE State (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                workflowId INTEGER NOT NULL,
                caption TEXT NOT NULL,
                description TEXT,
                weight INTEGER DEFAULT 0,
                orderNumber INTEGER DEFAULT 0,
                isArchive INTEGER NOT NULL DEFAULT 0,
                isQueue INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (workflowId) REFERENCES Workflow(id) ON DELETE CASCADE
            )
        )");

        conn->execute(R"(
            CREATE TABLE Edge (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                beginStateId INTEGER NOT NULL,
                endStateId INTEGER NOT NULL,
                FOREIGN KEY (beginStateId) REFERENCES State(id) ON DELETE CASCADE,
                FOREIGN KEY (endStateId) REFERENCES State(id) ON DELETE CASCADE,
                UNIQUE(beginStateId, endStateId)
            )
        )");

        // Создаем тестовый workflow с состояниями
        conn->execute(
            "INSERT INTO Workflow (caption) VALUES ('Test Workflow')"
        );
        m_testWorkflowId = conn->lastInsertId();

        // Создаем состояния
        m_newStateId = createState("Новая", 1);
        m_inProgressStateId = createState("В работе", 2);
        m_reviewStateId = createState("На проверке", 3);
        m_doneStateId = createState("Завершена", 4);
        m_archiveStateId = createState("Архив", 5);

        m_repository = std::make_unique<repositories::SqliteEdgeRepository>(m_database);
    }

    int64_t createState(const std::string& caption, int orderNumber)
    {
        auto conn = m_database->connection();
        auto stmt = conn->prepareStatement(
            "INSERT INTO State (workflowId, caption, orderNumber) "
            "VALUES (:workflowId, :caption, :orderNumber)"
        );

        stmt->bindInt64("workflowId", m_testWorkflowId);
        stmt->bindString("caption", caption);
        stmt->bindInt64("orderNumber", orderNumber);
        stmt->execute();

        return conn->lastInsertId();
    }

    dto::Edge createTestEdge(int64_t beginStateId, int64_t endStateId)
    {
        dto::Edge edge;
        edge.beginStateId = beginStateId;
        edge.endStateId = endStateId;
        return edge;
    }

    ~EdgeRepositoryFixture()
    {
        m_repository.reset();

        if (m_database)
        {
            m_database->shutdown();
            m_database.reset();
        }

        if (!m_tempDbPath.empty() && std::filesystem::exists(m_tempDbPath))
        {
            std::error_code ec;
            std::filesystem::remove(m_tempDbPath, ec);
        }
    }

    std::filesystem::path m_tempDbPath;
    std::shared_ptr<db::SqliteDatabase> m_database;
    std::unique_ptr<repositories::SqliteEdgeRepository> m_repository;
    int64_t m_testWorkflowId = 0;
    int64_t m_newStateId = 0;
    int64_t m_inProgressStateId = 0;
    int64_t m_reviewStateId = 0;
    int64_t m_doneStateId = 0;
    int64_t m_archiveStateId = 0;
};

BOOST_FIXTURE_TEST_SUITE(SqliteEdgeRepositoryTests, EdgeRepositoryFixture)

BOOST_AUTO_TEST_CASE(test_create_edge_success)
{
    // Arrange
    auto edge = createTestEdge(m_newStateId, m_inProgressStateId);

    // Act
    int64_t edgeId = m_repository->create(edge);

    // Assert
    BOOST_CHECK_GT(edgeId, 0);
    BOOST_CHECK(m_repository->exists(m_newStateId, m_inProgressStateId));
}

BOOST_AUTO_TEST_CASE(test_create_edge_duplicate_fails)
{
    // Arrange
    auto edge = createTestEdge(m_newStateId, m_inProgressStateId);
    m_repository->create(edge);

    // Act & Assert - попытка создать дубликат должна бросить исключение
    BOOST_CHECK_THROW(
        m_repository->create(edge),
        std::exception
    );
}

BOOST_AUTO_TEST_CASE(test_create_edge_missing_required_fields)
{
    // Arrange - без beginStateId
    dto::Edge edge;
    edge.endStateId = m_inProgressStateId;

    // Act
    int64_t edgeId = m_repository->create(edge);

    // Assert
    BOOST_CHECK_EQUAL(edgeId, 0);
}

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto edge = createTestEdge(m_newStateId, m_inProgressStateId);
    int64_t edgeId = m_repository->create(edge);
    BOOST_REQUIRE_GT(edgeId, 0);

    // Act
    auto found = m_repository->findById(edgeId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), edgeId);
    BOOST_CHECK_EQUAL(found->beginStateId.value(), m_newStateId);
    BOOST_CHECK_EQUAL(found->endStateId.value(), m_inProgressStateId);
}

BOOST_AUTO_TEST_CASE(test_find_by_id_not_found)
{
    // Act
    auto found = m_repository->findById(99999);

    // Assert
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_delete_edge_success)
{
    // Arrange
    auto edge = createTestEdge(m_newStateId, m_inProgressStateId);
    int64_t edgeId = m_repository->create(edge);
    BOOST_REQUIRE_GT(edgeId, 0);

    // Act
    bool result = m_repository->remove(edgeId);

    // Assert
    BOOST_CHECK(result);
    BOOST_CHECK(!m_repository->exists(m_newStateId, m_inProgressStateId));
}

BOOST_AUTO_TEST_CASE(test_delete_edge_nonexistent)
{
    // Act
    bool result = m_repository->remove(99999);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_exists_true)
{
    // Arrange
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));

    // Act
    bool exists = m_repository->exists(m_inProgressStateId, m_reviewStateId);

    // Assert
    BOOST_CHECK(exists);
}

BOOST_AUTO_TEST_CASE(test_exists_false)
{
    // Act
    bool exists = m_repository->exists(m_newStateId, m_doneStateId);

    // Assert
    BOOST_CHECK(!exists);
}

BOOST_AUTO_TEST_CASE(test_find_by_state_id)
{
    // Arrange - создаем переходы, связанные с состоянием
    m_repository->create(createTestEdge(m_newStateId, m_inProgressStateId));
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));
    m_repository->create(createTestEdge(m_doneStateId, m_archiveStateId));

    // Act - ищем переходы, где состояние является начальным или конечным
    auto edges = m_repository->findByStateId(m_inProgressStateId);

    // Assert - должно найти 2 перехода (исходящий и входящий)
    BOOST_CHECK_EQUAL(edges.size(), 2);

    // Проверяем, что нашли оба перехода
    bool foundOutgoing = false;
    bool foundIncoming = false;
    for (const auto& edge : edges)
    {
        if (edge.beginStateId.value() == m_newStateId
            && edge.endStateId.value() == m_inProgressStateId)
        {
            foundIncoming = true;
        }
        if (edge.beginStateId.value() == m_inProgressStateId
            && edge.endStateId.value() == m_reviewStateId)
        {
            foundOutgoing = true;
        }
    }
    BOOST_CHECK(foundIncoming);
    BOOST_CHECK(foundOutgoing);
}

BOOST_AUTO_TEST_CASE(test_find_by_state_id_no_edges)
{
    // Arrange - состояние без переходов
    // Act
    auto edges = m_repository->findByStateId(m_archiveStateId);

    // Assert
    BOOST_CHECK(edges.empty());
}

BOOST_AUTO_TEST_CASE(test_find_by_workflow_id)
{
    // Arrange - создаем переходы в тестовом workflow
    m_repository->create(createTestEdge(m_newStateId, m_inProgressStateId));
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));
    m_repository->create(createTestEdge(m_reviewStateId, m_doneStateId));

    // Act
    auto edges = m_repository->findByWorkflowId(m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(edges.size(), 3);

    // Проверяем, что все переходы относятся к нашему workflow
    for (const auto& edge : edges)
    {
        BOOST_CHECK(
            (edge.beginStateId.value() == m_newStateId
             || edge.beginStateId.value() == m_inProgressStateId
             || edge.beginStateId.value() == m_reviewStateId)
        );
        BOOST_CHECK(
            (edge.endStateId.value() == m_inProgressStateId
             || edge.endStateId.value() == m_reviewStateId
             || edge.endStateId.value() == m_doneStateId)
        );
    }
}

BOOST_AUTO_TEST_CASE(test_find_by_workflow_id_empty)
{
    // Act
    auto edges = m_repository->findByWorkflowId(99999);

    // Assert
    BOOST_CHECK(edges.empty());
}

BOOST_AUTO_TEST_CASE(test_find_all_with_filters)
{
    // Arrange
    m_repository->create(createTestEdge(m_newStateId, m_inProgressStateId));
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));
    m_repository->create(createTestEdge(m_reviewStateId, m_doneStateId));

    // Act - фильтр по начальному состоянию
    auto [edges, totalCount] = m_repository->findAll(1, 10, m_newStateId, std::nullopt);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 1);
    BOOST_CHECK_EQUAL(edges.size(), 1);
    BOOST_CHECK_EQUAL(edges[0].beginStateId.value(), m_newStateId);
    BOOST_CHECK_EQUAL(edges[0].endStateId.value(), m_inProgressStateId);

    // Act - фильтр по конечному состоянию
    auto [edges2, totalCount2] = m_repository->findAll(1, 10, std::nullopt, m_doneStateId);

    // Assert
    BOOST_CHECK_EQUAL(totalCount2, 1);
    BOOST_CHECK_EQUAL(edges2[0].beginStateId.value(), m_reviewStateId);
    BOOST_CHECK_EQUAL(edges2[0].endStateId.value(), m_doneStateId);
}

BOOST_AUTO_TEST_CASE(test_find_all_pagination)
{
    // Arrange
    m_repository->create(createTestEdge(m_newStateId, m_inProgressStateId));
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));
    m_repository->create(createTestEdge(m_reviewStateId, m_doneStateId));

    // Act - первая страница, 2 элемента
    auto [edges, totalCount] = m_repository->findAll(1, 2);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 3);
    BOOST_CHECK_EQUAL(edges.size(), 2);

    // Act - вторая страница
    auto [edges2, totalCount2] = m_repository->findAll(2, 2);

    // Assert
    BOOST_CHECK_EQUAL(totalCount2, 3);
    BOOST_CHECK_EQUAL(edges2.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_create_self_referencing_edge)
{
    // Arrange - пытаемся создать переход из состояния в себя
    auto edge = createTestEdge(m_newStateId, m_newStateId);

    // Act & Assert - должно бросить исключение из-за UNIQUE constraint
    // (или может быть разрешено, зависит от бизнес-логики)
    // В данном случае UNIQUE constraint позволяет это,
    // но в реальном приложении стоит добавить проверку
    BOOST_CHECK_NO_THROW(m_repository->create(edge));
}

BOOST_AUTO_TEST_CASE(test_complete_workflow_cycle)
{
    // Arrange - создаем полный цикл переходов
    m_repository->create(createTestEdge(m_newStateId, m_inProgressStateId));
    m_repository->create(createTestEdge(m_inProgressStateId, m_reviewStateId));
    m_repository->create(createTestEdge(m_reviewStateId, m_doneStateId));
    m_repository->create(createTestEdge(m_reviewStateId, m_inProgressStateId)); // возврат
    m_repository->create(createTestEdge(m_doneStateId, m_archiveStateId));

    // Act - получаем все переходы workflow
    auto edges = m_repository->findByWorkflowId(m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(edges.size(), 5);

    // Проверяем, что можно получить переходы по каждому состоянию
    auto newStateEdges = m_repository->findByStateId(m_newStateId);
    BOOST_CHECK_EQUAL(newStateEdges.size(), 1);

    auto inProgressEdges = m_repository->findByStateId(m_inProgressStateId);
    BOOST_CHECK_EQUAL(inProgressEdges.size(), 3); // входящий + 2 исходящих

    auto reviewEdges = m_repository->findByStateId(m_reviewStateId);
    BOOST_CHECK_EQUAL(reviewEdges.size(), 3); // входящий + 2 исходящих
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
