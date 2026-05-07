#include <cstdio>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "common/dto/workflow.h"

#include "repo/sqlite/sqlite_workflow_repository.h"
#include "storage/database_factory.h"
#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct WorkflowRepositoryFixture
{
    WorkflowRepositoryFixture()
    {
        // Создаем временный файл для тестовой БД
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_workflow_repo.db";

        // Удаляем файл если он существует от предыдущего запуска
        std::remove(m_tempDbPath.c_str());

        // Создаем файловую БД
        db::DatabaseConfig config;
        config["database"] = m_tempDbPath.string();

        m_database = std::make_shared<db::SqliteDatabase>();
        m_database->initialize(config);

        // Создаем схему
        auto conn = m_database->connection();
        conn->execute(R"(
            CREATE TABLE Workflow (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                caption TEXT NOT NULL,
                description TEXT
            )
        )");

        // Создаем репозиторий
        m_repository = std::make_unique<repositories::SqliteWorkflowRepository>(m_database);
    }

    /**
     * @brief Создает тестовый рабочий процесс.
     */
    dto::Workflow createTestWorkflow(
        const std::string& caption = "Тестовый процесс",
        const std::string& description = "Описание тестового процесса"
    )
    {
        dto::Workflow workflow;
        workflow.caption = caption;
        workflow.description = description;
        return workflow;
    }

    ~WorkflowRepositoryFixture()
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
    std::unique_ptr<repositories::SqliteWorkflowRepository> m_repository;
};

BOOST_FIXTURE_TEST_SUITE(SqliteWorkflowRepositoryTests, WorkflowRepositoryFixture)

BOOST_AUTO_TEST_CASE(test_create_workflow_success)
{
    // Arrange
    auto workflow = createTestWorkflow();

    // Act
    int64_t workflowId = m_repository->create(workflow);

    // Assert
    BOOST_CHECK_GT(workflowId, 0);
    BOOST_CHECK(m_repository->existsByCaption("Тестовый процесс"));
}

BOOST_AUTO_TEST_CASE(test_create_workflow_minimal)
{
    // Arrange - только обязательное поле caption
    dto::Workflow workflow;
    workflow.caption = "Минимальный процесс";

    // Act
    int64_t workflowId = m_repository->create(workflow);

    // Assert
    BOOST_CHECK_GT(workflowId, 0);

    auto found = m_repository->findById(workflowId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->caption.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Минимальный процесс");
    BOOST_CHECK(!found->description.has_value());
}

BOOST_AUTO_TEST_CASE(test_create_workflow_missing_caption)
{
    // Arrange
    dto::Workflow workflow; // Без caption

    // Act
    int64_t workflowId = m_repository->create(workflow);

    // Assert
    BOOST_CHECK_EQUAL(workflowId, 0);
}

BOOST_AUTO_TEST_CASE(test_create_workflow_empty_caption)
{
    // Arrange
    dto::Workflow workflow;
    workflow.caption = "";

    // Act
    int64_t workflowId = m_repository->create(workflow);

    // Assert
    BOOST_CHECK_EQUAL(workflowId, 0);
}

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto workflow = createTestWorkflow("Agile Process", "Описание Agile");
    int64_t workflowId = m_repository->create(workflow);
    BOOST_REQUIRE_GT(workflowId, 0);

    // Act
    auto found = m_repository->findById(workflowId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->id.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), workflowId);
    BOOST_CHECK(found->caption.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Agile Process");
    BOOST_CHECK(found->description.has_value());
    BOOST_CHECK_EQUAL(found->description.value(), "Описание Agile");
}

BOOST_AUTO_TEST_CASE(test_find_by_id_not_found)
{
    // Act
    auto found = m_repository->findById(99999);

    // Assert
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_find_by_id_invalid_returns_nullopt)
{
    // Act
    auto found = m_repository->findById(-1);

    // Assert
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_update_workflow_success)
{
    // Arrange
    auto workflow = createTestWorkflow("Old Name", "Old Description");
    int64_t workflowId = m_repository->create(workflow);
    BOOST_REQUIRE_GT(workflowId, 0);

    // Act - обновляем название и описание
    dto::Workflow updatedWorkflow;
    updatedWorkflow.id = workflowId;
    updatedWorkflow.caption = "New Name";
    updatedWorkflow.description = "New Description";

    bool result = m_repository->update(updatedWorkflow);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(workflowId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "New Name");
    BOOST_CHECK_EQUAL(found->description.value(), "New Description");
}

BOOST_AUTO_TEST_CASE(test_update_workflow_partial)
{
    // Arrange
    auto workflow = createTestWorkflow("Original", "Original Description");
    int64_t workflowId = m_repository->create(workflow);
    BOOST_REQUIRE_GT(workflowId, 0);

    // Act - обновляем только название
    dto::Workflow updatedWorkflow;
    updatedWorkflow.id = workflowId;
    updatedWorkflow.caption = "Updated Name";

    bool result = m_repository->update(updatedWorkflow);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(workflowId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Updated Name");
    // Описание должно остаться прежним
    BOOST_CHECK(found->description.has_value());
    BOOST_CHECK_EQUAL(found->description.value(), "Original Description");
}

BOOST_AUTO_TEST_CASE(test_update_workflow_nonexistent)
{
    // Arrange
    dto::Workflow workflow;
    workflow.id = 99999;
    workflow.caption = "Ghost";

    // Act
    bool result = m_repository->update(workflow);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_update_workflow_no_fields)
{
    // Arrange
    auto workflow = createTestWorkflow();
    int64_t workflowId = m_repository->create(workflow);
    BOOST_REQUIRE_GT(workflowId, 0);

    dto::Workflow emptyUpdate;
    emptyUpdate.id = workflowId;

    // Act
    bool result = m_repository->update(emptyUpdate);

    // Assert
    BOOST_CHECK(!result); // Нечего обновлять
}

BOOST_AUTO_TEST_CASE(test_delete_workflow_success)
{
    // Arrange
    auto workflow = createTestWorkflow();
    int64_t workflowId = m_repository->create(workflow);
    BOOST_REQUIRE_GT(workflowId, 0);

    // Act
    bool result = m_repository->remove(workflowId);

    // Assert
    BOOST_CHECK(result);

    // Проверяем, что запись действительно удалена
    auto found = m_repository->findById(workflowId);
    BOOST_CHECK(!found.has_value());
}

BOOST_AUTO_TEST_CASE(test_delete_workflow_nonexistent)
{
    // Act
    bool result = m_repository->remove(99999);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_exists_by_caption_true)
{
    // Arrange
    auto workflow = createTestWorkflow("Scrum Process");
    m_repository->create(workflow);

    // Act
    bool exists = m_repository->existsByCaption("Scrum Process");

    // Assert
    BOOST_CHECK(exists);
}

BOOST_AUTO_TEST_CASE(test_exists_by_caption_false)
{
    // Act
    bool exists = m_repository->existsByCaption("Non-existent Process");

    // Assert
    BOOST_CHECK(!exists);
}

BOOST_AUTO_TEST_CASE(test_exists_by_caption_empty_returns_false)
{
    // Act
    bool exists = m_repository->existsByCaption("");

    // Assert
    BOOST_CHECK(!exists);
}

BOOST_AUTO_TEST_CASE(test_find_all_with_pagination)
{
    // Arrange - создаем несколько рабочих процессов
    m_repository->create(createTestWorkflow("Process 1"));
    m_repository->create(createTestWorkflow("Process 2"));
    m_repository->create(createTestWorkflow("Process 3"));

    // Act - получаем вторую страницу с размером 1
    auto [workflows, totalCount] = m_repository->findAll(2, 1);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 3);
    BOOST_CHECK_EQUAL(workflows.size(), 1);
    BOOST_CHECK(workflows[0].caption.has_value());
    BOOST_CHECK_EQUAL(workflows[0].caption.value(), "Process 2");
}

BOOST_AUTO_TEST_CASE(test_find_all_empty)
{
    // Act
    auto [workflows, totalCount] = m_repository->findAll(1, 20);

    // Assert
    BOOST_CHECK_EQUAL(totalCount, 0);
    BOOST_CHECK(workflows.empty());
}

BOOST_AUTO_TEST_CASE(test_multiple_workflows_independent)
{
    // Arrange
    auto workflow1 = createTestWorkflow("Workflow 1", "Description 1");
    auto workflow2 = createTestWorkflow("Workflow 2", "Description 2");

    // Act
    int64_t id1 = m_repository->create(workflow1);
    int64_t id2 = m_repository->create(workflow2);

    // Assert
    BOOST_CHECK_GT(id1, 0);
    BOOST_CHECK_GT(id2, 0);
    BOOST_CHECK_NE(id1, id2);

    auto found1 = m_repository->findById(id1);
    auto found2 = m_repository->findById(id2);

    BOOST_REQUIRE(found1.has_value());
    BOOST_REQUIRE(found2.has_value());

    BOOST_CHECK_EQUAL(found1->caption.value(), "Workflow 1");
    BOOST_CHECK_EQUAL(found2->caption.value(), "Workflow 2");
    BOOST_CHECK_EQUAL(found1->description.value(), "Description 1");
    BOOST_CHECK_EQUAL(found2->description.value(), "Description 2");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
