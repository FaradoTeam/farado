#include <cstdio>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "common/dto/state.h"

#include "repo/sqlite/sqlite_state_repository.h"
#include "storage/database_factory.h"
#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct StateRepositoryFixture
{
    StateRepositoryFixture()
    {
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_state_repo.db";
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
            CREATE TABLE Item (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                itemTypeId INTEGER NOT NULL,
                parentId INTEGER,
                stateId INTEGER NOT NULL,
                phaseId INTEGER,
                caption TEXT NOT NULL,
                content TEXT,
                searchCaption TEXT,
                searchContent TEXT,
                isDeleted INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (stateId) REFERENCES State(id) ON DELETE CASCADE
            )
        )");

        // Создаем тестовый workflow
        conn->execute(
            "INSERT INTO Workflow (caption, description) "
            "VALUES ('Test Workflow', 'For testing')"
        );
        m_testWorkflowId = conn->lastInsertId();

        m_repository = std::make_unique<repositories::SqliteStateRepository>(m_database);
    }

    dto::State createTestState(
        const std::string& caption = "Новая",
        int64_t workflowId = -1,
        int weight = 0,
        int orderNumber = 1,
        bool isQueue = false
    )
    {
        dto::State state;
        state.workflowId = (workflowId > 0) ? workflowId : m_testWorkflowId;
        state.caption = caption;
        state.weight = weight;
        state.orderNumber = orderNumber;
        state.isArchive = false;
        state.isQueue = isQueue;
        return state;
    }

    ~StateRepositoryFixture()
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
    std::unique_ptr<repositories::SqliteStateRepository> m_repository;
    int64_t m_testWorkflowId = 0;
};

BOOST_FIXTURE_TEST_SUITE(SqliteStateRepositoryTests, StateRepositoryFixture)

BOOST_AUTO_TEST_CASE(test_create_state_success)
{
    // Arrange
    auto state = createTestState("В работе", m_testWorkflowId, 50, 2);

    // Act
    int64_t stateId = m_repository->create(state);

    // Assert
    BOOST_CHECK_GT(stateId, 0);

    auto found = m_repository->findById(stateId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "В работе");
    BOOST_CHECK_EQUAL(found->weight.value(), 50);
    BOOST_CHECK_EQUAL(found->orderNumber.value(), 2);
    BOOST_CHECK(!found->isQueue.value());
}

BOOST_AUTO_TEST_CASE(test_create_state_with_all_fields)
{
    // Arrange
    auto state = createTestState("На проверке", m_testWorkflowId, 75, 3, true);
    state.description = "Состояние проверки";
    state.isArchive = false;

    // Act
    int64_t stateId = m_repository->create(state);

    // Assert
    BOOST_CHECK_GT(stateId, 0);

    auto found = m_repository->findById(stateId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "На проверке");
    BOOST_CHECK_EQUAL(found->description.value(), "Состояние проверки");
    BOOST_CHECK_EQUAL(found->weight.value(), 75);
    BOOST_CHECK_EQUAL(found->orderNumber.value(), 3);
    BOOST_CHECK(found->isQueue.value());
}

BOOST_AUTO_TEST_CASE(test_create_state_missing_required_fields)
{
    // Arrange - без workflowId
    dto::State state;
    state.caption = "Test";

    // Act
    int64_t stateId = m_repository->create(state);

    // Assert
    BOOST_CHECK_EQUAL(stateId, 0);
}

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto state = createTestState("Завершена", m_testWorkflowId, 100, 4);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    // Act
    auto found = m_repository->findById(stateId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), stateId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Завершена");
    BOOST_CHECK_EQUAL(found->workflowId.value(), m_testWorkflowId);
}

BOOST_AUTO_TEST_CASE(test_find_by_id_not_found)
{
    // Act
    auto found = m_repository->findById(99999);

    // Assert
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_update_state_success)
{
    // Arrange
    auto state = createTestState("Old State", m_testWorkflowId);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    // Act
    dto::State updatedState;
    updatedState.id = stateId;
    updatedState.caption = "Updated State";
    updatedState.weight = 42;
    updatedState.orderNumber = 10;

    bool result = m_repository->update(updatedState);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(stateId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Updated State");
    BOOST_CHECK_EQUAL(found->weight.value(), 42);
    BOOST_CHECK_EQUAL(found->orderNumber.value(), 10);
}

BOOST_AUTO_TEST_CASE(test_update_state_archive_flag)
{
    // Arrange
    auto state = createTestState("To Archive", m_testWorkflowId);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    // Act - архивируем состояние
    dto::State archivedState;
    archivedState.id = stateId;
    archivedState.isArchive = true;

    bool result = m_repository->update(archivedState);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(stateId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->isArchive.value());
}

BOOST_AUTO_TEST_CASE(test_delete_state_success)
{
    // Arrange
    auto state = createTestState("To Delete", m_testWorkflowId);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    // Act
    bool result = m_repository->remove(stateId);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(stateId);
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_find_by_workflow_id)
{
    // Arrange - создаем несколько состояний в одном workflow
    m_repository->create(createTestState("Новая", m_testWorkflowId, 0, 1));
    m_repository->create(createTestState("В работе", m_testWorkflowId, 50, 2));
    m_repository->create(createTestState("Завершена", m_testWorkflowId, 100, 3));

    // Act
    auto states = m_repository->findByWorkflowId(m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(states.size(), 3);
    // Проверяем сортировку по orderNumber
    BOOST_CHECK_EQUAL(states[0].caption.value(), "Новая");
    BOOST_CHECK_EQUAL(states[1].caption.value(), "В работе");
    BOOST_CHECK_EQUAL(states[2].caption.value(), "Завершена");
    BOOST_CHECK_EQUAL(states[0].orderNumber.value(), 1);
    BOOST_CHECK_EQUAL(states[2].orderNumber.value(), 3);
}

BOOST_AUTO_TEST_CASE(test_find_by_workflow_id_empty)
{
    // Act
    auto states = m_repository->findByWorkflowId(99999);

    // Assert
    BOOST_CHECK(states.empty());
}

BOOST_AUTO_TEST_CASE(test_is_used_by_items_true)
{
    // Arrange - создаем состояние и привязываем к нему элемент
    auto state = createTestState("Used State", m_testWorkflowId);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    auto conn = m_database->connection();
    conn->execute(
        "INSERT INTO Item (itemTypeId, stateId, phaseId, caption) "
        "VALUES (1, " + std::to_string(stateId) + ", 1, 'Test Item')"
    );

    // Act
    bool isUsed = m_repository->isUsedByItems(stateId);

    // Assert
    BOOST_CHECK(isUsed);
}

BOOST_AUTO_TEST_CASE(test_is_used_by_items_false)
{
    // Arrange - создаем состояние без элементов
    auto state = createTestState("Unused State", m_testWorkflowId);
    int64_t stateId = m_repository->create(state);
    BOOST_REQUIRE_GT(stateId, 0);

    // Act
    bool isUsed = m_repository->isUsedByItems(stateId);

    // Assert
    BOOST_CHECK(!isUsed);
}

BOOST_AUTO_TEST_CASE(test_find_all_with_workflow_filter)
{
    // Arrange - создаем второй workflow и состояния в обоих
    auto conn = m_database->connection();
    conn->execute(
        "INSERT INTO Workflow (caption) VALUES ('Second Workflow')"
    );
    int64_t secondWorkflowId = conn->lastInsertId();

    m_repository->create(createTestState("State 1", m_testWorkflowId, 0, 1));
    m_repository->create(createTestState("State 2", m_testWorkflowId, 0, 2));
    m_repository->create(createTestState("State 3", secondWorkflowId, 0, 1));

    // Act - получаем состояния первого workflow
    auto [states, totalCount] = m_repository->findAll(1, 10, m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 2);
    BOOST_CHECK_EQUAL(states.size(), 2);
    BOOST_CHECK_EQUAL(states[0].caption.value(), "State 1");
    BOOST_CHECK_EQUAL(states[1].caption.value(), "State 2");
}

BOOST_AUTO_TEST_CASE(test_find_all_pagination)
{
    // Arrange
    for (int i = 1; i <= 5; ++i)
    {
        m_repository->create(
            createTestState("State " + std::to_string(i), m_testWorkflowId, 0, i)
        );
    }

    // Act - первая страница, 2 элемента
    auto [states, totalCount] = m_repository->findAll(1, 2);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 5);
    BOOST_CHECK_EQUAL(states.size(), 2);
    BOOST_CHECK_EQUAL(states[0].caption.value(), "State 1");
    BOOST_CHECK_EQUAL(states[1].caption.value(), "State 2");

    // Act - вторая страница
    auto [states2, totalCount2] = m_repository->findAll(2, 2);

    // Assert
    BOOST_CHECK_EQUAL(totalCount2, 5);
    BOOST_CHECK_EQUAL(states2.size(), 2);
    BOOST_CHECK_EQUAL(states2[0].caption.value(), "State 3");
    BOOST_CHECK_EQUAL(states2[1].caption.value(), "State 4");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
