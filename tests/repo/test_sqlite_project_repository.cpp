#include <cstdio>
#include <filesystem>
#include <thread>

#include <boost/test/unit_test.hpp>

#include "common/dto/project.h"

#include "repo/sqlite/sqlite_project_repository.h"

#include "storage/database_factory.h"
#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct ProjectRepositoryFixture
{
    ProjectRepositoryFixture()
    {
        // Создаем временный файл для тестовой БД
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_project_repo.db";

        // Удаляем файл если он существует от предыдущего запуска
        std::remove(m_tempDbPath.c_str());

        // Создаем файловую БД
        db::DatabaseConfig config;
        config["database"] = m_tempDbPath.string();

        m_database = std::make_shared<db::SqliteDatabase>();
        m_database->initialize(config);

        // Используем ОДНО соединение для создания схемы
        auto conn = m_database->connection();

        // Создаем схему
        conn->execute(R"(
            CREATE TABLE Project (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                parentId INTEGER,
                caption TEXT NOT NULL,
                description TEXT,
                searchCaption TEXT,
                searchDescription TEXT,
                createdAt INTEGER NOT NULL,
                updatedAt INTEGER NOT NULL,
                isArchive INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (parentId) REFERENCES Project(id) ON DELETE SET NULL
            )
        )");

        // Создаем репозиторий с тем же объектом базы данных
        m_repository = std::make_unique<repositories::SqliteProjectRepository>(m_database);
    }

    /**
     * @brief Создает тестовый проект.
     */
    dto::Project createTestProject(
        const std::string& caption = "Тестовый проект",
        std::optional<int64_t> parentId = std::nullopt
    )
    {
        dto::Project project;
        project.caption = caption;
        project.description = "Описание тестового проекта";
        project.parentId = parentId;
        project.isArchive = false;
        return project;
    }

    ~ProjectRepositoryFixture()
    {
        // Важно: сначала уничтожаем репозиторий, потом БД
        m_repository.reset();

        if (m_database)
        {
            m_database->shutdown();
            m_database.reset();
        }

        // Удаляем временный файл БД
        if (!m_tempDbPath.empty() && std::filesystem::exists(m_tempDbPath))
        {
            std::error_code ec;
            std::filesystem::remove(m_tempDbPath, ec);
        }
    }

    std::filesystem::path m_tempDbPath;
    std::shared_ptr<db::SqliteDatabase> m_database;
    std::unique_ptr<repositories::SqliteProjectRepository> m_repository;
};

BOOST_FIXTURE_TEST_SUITE(SqliteProjectRepositoryTests, ProjectRepositoryFixture)

// ============================================================
// Тесты создания проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_create_project_success)
{
    // Arrange
    auto project = createTestProject();

    // Act
    int64_t projectId = m_repository->create(project);

    // Assert
    BOOST_CHECK_GT(projectId, 0);
    BOOST_CHECK(m_repository->exists(projectId));

    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->caption.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Тестовый проект");
    BOOST_CHECK(found->description.has_value());
    BOOST_CHECK_EQUAL(found->description.value(), "Описание тестового проекта");
    BOOST_CHECK(!found->parentId.has_value());
    BOOST_CHECK(found->isArchive.has_value());
    BOOST_CHECK(!found->isArchive.value());
    BOOST_CHECK(found->createdAt.has_value());
    BOOST_CHECK(found->updatedAt.has_value());
}

BOOST_AUTO_TEST_CASE(test_create_project_with_parent)
{
    // Arrange
    auto parentProject = createTestProject("Родительский проект");
    int64_t parentId = m_repository->create(parentProject);
    BOOST_REQUIRE_GT(parentId, 0);

    auto childProject = createTestProject("Дочерний проект", parentId);

    // Act
    int64_t childId = m_repository->create(childProject);

    // Assert
    BOOST_CHECK_GT(childId, 0);

    auto found = m_repository->findById(childId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->parentId.has_value());
    BOOST_CHECK_EQUAL(found->parentId.value(), parentId);
}

BOOST_AUTO_TEST_CASE(test_create_project_without_caption_fails)
{
    // Arrange
    dto::Project project;
    project.description = "Проект без названия";

    // Act & Assert
    int64_t result = m_repository->create(project);
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_project_empty_caption_fails)
{
    // Arrange
    auto project = createTestProject("");

    // Act
    int64_t result = m_repository->create(project);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_project_search_fields_generated)
{
    // Arrange
    auto project = createTestProject("ПРОЕКТ Для Поиска");
    project.description = "ОПИСАНИЕ для Поиска";

    // Act
    int64_t projectId = m_repository->create(project);
    BOOST_REQUIRE_GT(projectId, 0);

    // Assert - проверяем поисковые поля напрямую в БД
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "SELECT caption, description, searchCaption, searchDescription FROM Project WHERE id = :id"
    );
    stmt->bindInt64("id", projectId);
    auto rs = stmt->executeQuery();

    BOOST_REQUIRE(rs->next());

    // Основные поля должны быть в оригинальном регистре
    BOOST_CHECK_EQUAL(rs->valueString("caption"), "ПРОЕКТ Для Поиска");

    // Поисковые поля должны быть в нижнем регистре
    BOOST_CHECK_EQUAL(rs->valueString("searchCaption"), "проект для поиска");
    BOOST_CHECK_EQUAL(rs->valueString("searchDescription"), "описание для поиска");
}

// ============================================================
// Тесты поиска проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto project = createTestProject("Проект для поиска");
    int64_t projectId = m_repository->create(project);
    BOOST_REQUIRE_GT(projectId, 0);

    // Act
    auto found = m_repository->findById(projectId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->id.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), projectId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Проект для поиска");
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

// ============================================================
// Тесты получения списка проектов с пагинацией
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_all_empty)
{
    // Act
    auto [projects, total] = m_repository->findAll(1, 20);

    // Assert
    BOOST_CHECK_EQUAL(total, 0);
    BOOST_CHECK(projects.empty());
}

BOOST_AUTO_TEST_CASE(test_find_all_with_pagination)
{
    // Arrange - создаем 15 проектов
    for (int i = 1; i <= 15; ++i)
    {
        auto project = createTestProject("Проект " + std::to_string(i));
        m_repository->create(project);
    }

    // Act - первая страница (10 элементов)
    auto [page1, total] = m_repository->findAll(1, 10);

    // Assert
    BOOST_CHECK_EQUAL(total, 15);
    BOOST_CHECK_EQUAL(page1.size(), 10);

    // Act - вторая страница (5 элементов)
    auto [page2, total2] = m_repository->findAll(2, 10);
    BOOST_CHECK_EQUAL(page2.size(), 5);
    BOOST_CHECK_EQUAL(total2, 15);

    // Проверяем, что элементы на разных страницах
    BOOST_CHECK_NE(page1[0].id.value(), page2[0].id.value());
}

BOOST_AUTO_TEST_CASE(test_find_all_by_parent)
{
    // Arrange
    auto parentProject = createTestProject("Родитель");
    int64_t parentId = m_repository->create(parentProject);

    for (int i = 0; i < 3; ++i)
    {
        auto child = createTestProject("Дочерний " + std::to_string(i), parentId);
        m_repository->create(child);
    }
    // Создаем еще один проект без родителя
    m_repository->create(createTestProject("Без родителя"));

    // Act - ищем проекты с родителем
    auto [children, total] = m_repository->findAll(1, 20, parentId);

    // Assert
    BOOST_CHECK_EQUAL(total, 3);
    BOOST_CHECK_EQUAL(children.size(), 3);
    for (const auto& child : children)
    {
        BOOST_CHECK(child.parentId.has_value());
        BOOST_CHECK_EQUAL(child.parentId.value(), parentId);
    }
}

BOOST_AUTO_TEST_CASE(test_find_all_root_projects)
{
    // Arrange
    m_repository->create(createTestProject("Корневой 1"));
    m_repository->create(createTestProject("Корневой 2"));

    auto parentProject = createTestProject("Родитель");
    int64_t parentId = m_repository->create(parentProject);
    m_repository->create(createTestProject("Дочерний", parentId));

    // Act - ищем корневые проекты (parentId = 0)
    auto [rootProjects, total] = m_repository->findAll(1, 20, int64_t(0));

    // Assert
    BOOST_CHECK_EQUAL(total, 3); // 2 корневых + родитель
    BOOST_CHECK_EQUAL(rootProjects.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_find_all_archived_filter)
{
    // Arrange
    auto activeProject = createTestProject("Активный");
    int64_t activeId = m_repository->create(activeProject);

    auto archivedProject = createTestProject("Архивный");
    int64_t archivedId = m_repository->create(archivedProject);
    m_repository->archive(archivedId);

    // Act - только активные
    auto [active, totalActive] = m_repository->findAll(1, 20, std::nullopt, false);
    BOOST_CHECK_EQUAL(totalActive, 1);
    BOOST_CHECK_EQUAL(active[0].id.value(), activeId);

    // Act - только архивные
    auto [archived, totalArchived] = m_repository->findAll(1, 20, std::nullopt, true);
    BOOST_CHECK_EQUAL(totalArchived, 1);
    BOOST_CHECK_EQUAL(archived[0].id.value(), archivedId);

    // Act - все
    auto [all, totalAll] = m_repository->findAll(1, 20);
    BOOST_CHECK_EQUAL(totalAll, 2);
}

BOOST_AUTO_TEST_CASE(test_find_all_search_by_caption)
{
    // Arrange
    m_repository->create(createTestProject("Альфа проект"));
    m_repository->create(createTestProject("Бета тестирование"));
    m_repository->create(createTestProject("Гамма излучение"));

    // Act - поиск по слову "проект"
    auto [projects, total] = m_repository->findAll(1, 20, std::nullopt, std::nullopt, "проект");

    // Assert
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(projects[0].caption.value(), "Альфа проект");
}

BOOST_AUTO_TEST_CASE(test_find_all_search_case_insensitive)
{
    // Arrange
    auto project = createTestProject("ТЕСТОВЫЙ ПРОЕКТ");
    int64_t projectId = m_repository->create(project);
    BOOST_REQUIRE_GT(projectId, 0);

    // Act - поиск в нижнем регистре должен найти проект
    auto [projects, total] = m_repository->findAll(1, 20, std::nullopt, std::nullopt, "тестовый");

    // Assert
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(projects.size(), 1);
    BOOST_CHECK_EQUAL(projects[0].id.value(), projectId);
}

// ============================================================
// Тесты обновления проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_update_project_success)
{
    // Arrange
    auto project = createTestProject("Старое название");
    int64_t projectId = m_repository->create(project);
    BOOST_REQUIRE_GT(projectId, 0);

    // Act
    dto::Project updateData;
    updateData.id = projectId;
    updateData.caption = "Новое название";
    updateData.description = "Новое описание";
    updateData.isArchive = true;

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Новое название");
    BOOST_CHECK_EQUAL(found->description.value(), "Новое описание");
    BOOST_CHECK(found->isArchive.value());
}

BOOST_AUTO_TEST_CASE(test_update_project_partial)
{
    // Arrange
    auto project = createTestProject("Оригинал");
    project.description = "Оригинальное описание";
    int64_t projectId = m_repository->create(project);

    // Act - обновляем только название
    dto::Project updateData;
    updateData.id = projectId;
    updateData.caption = "Обновленное название";

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленное название");
    // Описание не должно измениться
    BOOST_CHECK_EQUAL(found->description.value(), "Оригинальное описание");
}

BOOST_AUTO_TEST_CASE(test_update_project_search_fields_updated)
{
    // Arrange
    auto project = createTestProject("Старый Заголовок");
    int64_t projectId = m_repository->create(project);

    // Act
    dto::Project updateData;
    updateData.id = projectId;
    updateData.caption = "НОВЫЙ Заголовок";

    BOOST_CHECK(m_repository->update(updateData));

    // Assert - проверяем обновление поискового поля
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "SELECT searchCaption FROM Project WHERE id = :id"
    );
    stmt->bindInt64("id", projectId);
    auto rs = stmt->executeQuery();
    BOOST_REQUIRE(rs->next());
    BOOST_CHECK_EQUAL(rs->valueString("searchCaption"), "новый заголовок");
}

BOOST_AUTO_TEST_CASE(test_update_project_nonexistent)
{
    // Arrange
    dto::Project updateData;
    updateData.id = 99999;
    updateData.caption = "Несуществующий проект";

    // Act
    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_update_project_updated_at_changed)
{
    // Arrange
    auto project = createTestProject("Проект с временем");
    int64_t projectId = m_repository->create(project);

    auto original = m_repository->findById(projectId);
    auto originalTime = original->updatedAt.value();

    // Небольшая задержка для гарантии разницы во времени
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Act
    dto::Project updateData;
    updateData.id = projectId;
    updateData.caption = "Обновленное время";
    m_repository->update(updateData);

    // Assert
    auto updated = m_repository->findById(projectId);
    BOOST_CHECK(updated->updatedAt.value() > originalTime);
}

// ============================================================
// Тесты архивации и восстановления
// ============================================================

BOOST_AUTO_TEST_CASE(test_archive_project_success)
{
    // Arrange
    auto project = createTestProject("Проект для архивации");
    int64_t projectId = m_repository->create(project);

    // Act
    bool result = m_repository->archive(projectId);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->isArchive.value());
}

BOOST_AUTO_TEST_CASE(test_restore_project_success)
{
    // Arrange
    auto project = createTestProject("Проект для восстановления");
    int64_t projectId = m_repository->create(project);
    m_repository->archive(projectId);

    // Act
    bool result = m_repository->restore(projectId);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(!found->isArchive.value());
}

BOOST_AUTO_TEST_CASE(test_archive_nonexistent_project)
{
    // Act
    bool result = m_repository->archive(99999);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_restore_nonexistent_project)
{
    // Act
    bool result = m_repository->restore(99999);

    // Assert
    BOOST_CHECK(!result);
}

// ============================================================
// Тесты вспомогательных методов
// ============================================================

BOOST_AUTO_TEST_CASE(test_exists_true)
{
    // Arrange
    int64_t projectId = m_repository->create(createTestProject());

    // Act
    bool exists = m_repository->exists(projectId);

    // Assert
    BOOST_CHECK(exists);
}

BOOST_AUTO_TEST_CASE(test_exists_false)
{
    // Act
    bool exists = m_repository->exists(99999);

    // Assert
    BOOST_CHECK(!exists);
}

BOOST_AUTO_TEST_CASE(test_child_count)
{
    // Arrange
    auto parent = createTestProject("Родитель");
    int64_t parentId = m_repository->create(parent);

    for (int i = 0; i < 5; ++i)
    {
        auto child = createTestProject("Ребенок " + std::to_string(i), parentId);
        m_repository->create(child);
    }

    // Act
    int64_t count = m_repository->childCount(parentId);

    // Assert
    BOOST_CHECK_EQUAL(count, 5);
}

BOOST_AUTO_TEST_CASE(test_child_count_ignores_archived)
{
    // Arrange
    auto parent = createTestProject("Родитель");
    int64_t parentId = m_repository->create(parent);

    auto child1 = createTestProject("Активный ребенок", parentId);
    m_repository->create(child1);

    auto child2 = createTestProject("Архивный ребенок", parentId);
    int64_t child2Id = m_repository->create(child2);
    m_repository->archive(child2Id);

    // Act
    int64_t count = m_repository->childCount(parentId);

    // Assert
    BOOST_CHECK_EQUAL(count, 1); // Только активный
}

BOOST_AUTO_TEST_CASE(test_project_with_nullable_fields)
{
    // Arrange
    dto::Project project;
    project.caption = "Минимальный проект";
    // description, parentId остаются nullopt

    // Act
    int64_t projectId = m_repository->create(project);

    // Assert
    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(!found->description.has_value());
    BOOST_CHECK(!found->parentId.has_value());
}

BOOST_AUTO_TEST_CASE(test_multiple_projects_independent)
{
    // Arrange
    auto project1 = createTestProject("Проект 1");
    auto project2 = createTestProject("Проект 2");
    project2.description = "Второе описание";

    // Act
    int64_t id1 = m_repository->create(project1);
    int64_t id2 = m_repository->create(project2);

    // Assert
    BOOST_CHECK_GT(id1, 0);
    BOOST_CHECK_GT(id2, 0);
    BOOST_CHECK_NE(id1, id2);

    auto found1 = m_repository->findById(id1);
    auto found2 = m_repository->findById(id2);

    BOOST_REQUIRE(found1.has_value());
    BOOST_REQUIRE(found2.has_value());
    BOOST_CHECK_EQUAL(found1->caption.value(), "Проект 1");
    BOOST_CHECK_EQUAL(found1->description.value(), "Описание тестового проекта");
    BOOST_CHECK_EQUAL(found2->caption.value(), "Проект 2");
    BOOST_CHECK_EQUAL(found2->description.value(), "Второе описание");
}

// ============================================================
// Интеграционный тест: полный жизненный цикл проекта
// ============================================================

BOOST_AUTO_TEST_CASE(test_full_project_lifecycle)
{
    // 1. Создание
    auto project = createTestProject("Жизненный цикл");
    int64_t projectId = m_repository->create(project);
    BOOST_CHECK_GT(projectId, 0);

    // 2. Чтение
    auto found = m_repository->findById(projectId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Жизненный цикл");

    // 3. Обновление
    dto::Project updateData;
    updateData.id = projectId;
    updateData.caption = "Обновленный цикл";
    BOOST_CHECK(m_repository->update(updateData));

    // 4. Проверка обновления
    found = m_repository->findById(projectId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленный цикл");

    // 5. Архивация
    BOOST_CHECK(m_repository->archive(projectId));
    found = m_repository->findById(projectId);
    BOOST_CHECK(found->isArchive.value());

    // 6. Восстановление
    BOOST_CHECK(m_repository->restore(projectId));
    found = m_repository->findById(projectId);
    BOOST_CHECK(!found->isArchive.value());

    // 7. Проверка в списке
    auto [projects, total] = m_repository->findAll(1, 20);
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(projects[0].id.value(), projectId);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
