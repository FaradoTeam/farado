#include <cstdio>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "common/dto/item_type.h"

#include "repo/sqlite/sqlite_item_type_repository.h"

#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct ItemTypeRepositoryFixture
{
    ItemTypeRepositoryFixture()
    {
        // Создаем временный файл для тестовой БД
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_item_type_repo.db";

        // Удаляем файл если он существует от предыдущего запуска
        std::remove(m_tempDbPath.c_str());

        // Создаем файловую БД
        db::DatabaseConfig config;
        config["database"] = m_tempDbPath.string();

        m_database = std::make_shared<db::SqliteDatabase>();
        m_database->initialize(config);

        // Используем ОДНО соединение для создания схемы
        auto conn = m_database->connection();

        // Создаем схему для Workflow и ItemType
        conn->execute(R"(
            CREATE TABLE Workflow (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                caption TEXT NOT NULL,
                description TEXT
            )
        )");

        conn->execute(R"(
            CREATE TABLE ItemType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                workflowId INTEGER NOT NULL,
                defaultStateId INTEGER,
                caption TEXT NOT NULL,
                kind TEXT NOT NULL CHECK (kind IN ('issue', 'folder', 'test-case', 'test-cycle', 'test-execution', 'requirement')),
                defaultContent TEXT,
                FOREIGN KEY (workflowId) REFERENCES Workflow(id) ON DELETE CASCADE
            )
        )");

        // Создаем связанные таблицы для проверки usageCount
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
                FOREIGN KEY (itemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE
            )
        )");

        conn->execute(R"(
            CREATE TABLE FieldType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                itemTypeId INTEGER NOT NULL,
                caption TEXT NOT NULL,
                description TEXT,
                valueType TEXT NOT NULL,
                isBoardVisible INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (itemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE
            )
        )");

        conn->execute(R"(
            CREATE TABLE LinkType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                sourceItemTypeId INTEGER NOT NULL,
                destinationItemTypeId INTEGER NOT NULL,
                isBidirectional INTEGER NOT NULL DEFAULT 0,
                caption TEXT NOT NULL,
                FOREIGN KEY (sourceItemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE,
                FOREIGN KEY (destinationItemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE
            )
        )");

        conn->execute(R"(
            CREATE TABLE RuleItemType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                ruleId INTEGER NOT NULL,
                itemTypeId INTEGER NOT NULL,
                isReader INTEGER NOT NULL DEFAULT 0,
                isWriter INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (itemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE
            )
        )");

        // Создаем тестовый Workflow
        conn->execute(R"(
            INSERT INTO Workflow (caption, description)
            VALUES ('Тестовый процесс', 'Описание тестового процесса')
        )");
        m_testWorkflowId = conn->lastInsertId();

        // Создаем второй Workflow для тестирования фильтрации
        conn->execute(R"(
            INSERT INTO Workflow (caption, description)
            VALUES ('Второй процесс', 'Описание второго процесса')
        )");
        m_secondWorkflowId = conn->lastInsertId();

        // Создаем тестовые состояния для defaultStateId
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

        auto stmt = conn->prepareStatement(
            "INSERT INTO State (workflowId, caption, description) "
            "VALUES (:workflowId, 'Новая', 'Состояние по умолчанию')"
        );
        stmt->bindInt64("workflowId", m_testWorkflowId);
        stmt->execute();
        m_testDefaultStateId = conn->lastInsertId();

        // Создаем репозиторий
        m_repository = std::make_unique<repositories::SqliteItemTypeRepository>(m_database);
    }

    /**
     * @brief Создает тестовый тип элемента.
     */
    dto::ItemType createTestItemType(
        const std::string& caption = "Задача",
        const std::string& kind = "issue",
        std::optional<int64_t> workflowId = std::nullopt
    )
    {
        dto::ItemType itemType;
        itemType.caption = caption;
        itemType.kind = kind;
        itemType.workflowId = workflowId.has_value() ? workflowId.value() : m_testWorkflowId;
        itemType.defaultStateId = m_testDefaultStateId;
        itemType.defaultContent = "Содержимое по умолчанию";
        return itemType;
    }

    ~ItemTypeRepositoryFixture()
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
    std::unique_ptr<repositories::SqliteItemTypeRepository> m_repository;
    int64_t m_testWorkflowId = 0;
    int64_t m_testDefaultStateId = 0;
    int64_t m_secondWorkflowId = 0;
};

BOOST_FIXTURE_TEST_SUITE(SqliteItemTypeRepositoryTests, ItemTypeRepositoryFixture)

// ============================================================
// Тесты создания типа элемента
// ============================================================

BOOST_AUTO_TEST_CASE(test_create_item_type_success)
{
    // Arrange
    auto itemType = createTestItemType();

    // Act
    int64_t itemTypeId = m_repository->create(itemType);

    // Assert
    BOOST_CHECK_GT(itemTypeId, 0);
    BOOST_CHECK(m_repository->exists(itemTypeId));

    auto found = m_repository->findById(itemTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->caption.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Задача");
    BOOST_CHECK(found->kind.has_value());
    BOOST_CHECK_EQUAL(found->kind.value(), "issue");
    BOOST_CHECK(found->workflowId.has_value());
    BOOST_CHECK_EQUAL(found->workflowId.value(), m_testWorkflowId);
    BOOST_CHECK(found->defaultStateId.has_value());
    BOOST_CHECK_EQUAL(found->defaultStateId.value(), m_testDefaultStateId);
    BOOST_CHECK(found->defaultContent.has_value());
    BOOST_CHECK_EQUAL(found->defaultContent.value(), "Содержимое по умолчанию");
}

BOOST_AUTO_TEST_CASE(test_create_item_type_without_caption_fails)
{
    // Arrange
    dto::ItemType itemType;
    itemType.workflowId = m_testWorkflowId;
    itemType.kind = "issue";

    // Act
    int64_t result = m_repository->create(itemType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_item_type_invalid_kind_fails)
{
    // Arrange
    auto itemType = createTestItemType();
    itemType.kind = "invalid_kind";

    // Act
    int64_t result = m_repository->create(itemType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_item_type_all_valid_kinds)
{
    // Arrange
    std::vector<std::string> validKinds = {
        "issue", "folder", "test-case", "test-cycle", "test-execution", "requirement"
    };

    for (const auto& kind : validKinds)
    {
        auto itemType = createTestItemType("Type " + kind, kind);

        // Act
        int64_t id = m_repository->create(itemType);

        // Assert
        BOOST_CHECK_GT(id, 0);
        BOOST_CHECK(m_repository->exists(id));

        auto found = m_repository->findById(id);
        BOOST_REQUIRE(found.has_value());
        BOOST_CHECK_EQUAL(found->kind.value(), kind);
    }
}

BOOST_AUTO_TEST_CASE(test_create_item_type_without_workflow_fails)
{
    // Arrange
    dto::ItemType itemType;
    itemType.caption = "Тип без workflow";
    itemType.kind = "issue";

    // Act
    int64_t result = m_repository->create(itemType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_item_type_without_default_state)
{
    // Arrange
    auto itemType = createTestItemType();
    itemType.defaultStateId = std::nullopt;

    // Act
    int64_t id = m_repository->create(itemType);

    // Assert
    BOOST_CHECK_GT(id, 0);

    auto found = m_repository->findById(id);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(!found->defaultStateId.has_value());
}

// ============================================================
// Тесты поиска типа элемента
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto itemType = createTestItemType("Тип для поиска");
    int64_t itemTypeId = m_repository->create(itemType);
    BOOST_REQUIRE_GT(itemTypeId, 0);

    // Act
    auto found = m_repository->findById(itemTypeId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), itemTypeId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Тип для поиска");
    BOOST_CHECK_EQUAL(found->kind.value(), "issue");
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
// Тесты получения списка типов элементов с пагинацией
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_all_empty)
{
    // Act
    auto [itemTypes, total] = m_repository->findAll(1, 20);

    // Assert
    BOOST_CHECK_EQUAL(total, 0);
    BOOST_CHECK(itemTypes.empty());
}

BOOST_AUTO_TEST_CASE(test_find_all_with_pagination)
{
    // Arrange - создаем 15 типов элементов
    for (int i = 1; i <= 15; ++i)
    {
        m_repository->create(createTestItemType("Тип " + std::to_string(i)));
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

BOOST_AUTO_TEST_CASE(test_find_all_by_workflow)
{
    // Arrange
    m_repository->create(createTestItemType("Тип 1", "issue", m_testWorkflowId));
    m_repository->create(createTestItemType("Тип 2", "folder", m_testWorkflowId));
    m_repository->create(createTestItemType("Тип 3", "test-case", m_secondWorkflowId));

    // Act - фильтр по первому workflow
    auto [itemTypes1, total1] = m_repository->findAll(1, 20, m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(total1, 2);
    BOOST_CHECK_EQUAL(itemTypes1.size(), 2);

    // Act - фильтр по второму workflow
    auto [itemTypes2, total2] = m_repository->findAll(1, 20, m_secondWorkflowId);
    BOOST_CHECK_EQUAL(total2, 1);
    BOOST_CHECK_EQUAL(itemTypes2.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_find_all_by_kind)
{
    // Arrange
    m_repository->create(createTestItemType("Задача 1", "issue"));
    m_repository->create(createTestItemType("Задача 2", "issue"));
    m_repository->create(createTestItemType("Папка 1", "folder"));

    // Act - фильтр по kind
    auto [issues, totalIssues] = m_repository->findAll(1, 20, std::nullopt, std::string("issue"));
    auto [folders, totalFolders] = m_repository->findAll(1, 20, std::nullopt, std::string("folder"));

    // Assert
    BOOST_CHECK_EQUAL(totalIssues, 2);
    BOOST_CHECK_EQUAL(issues.size(), 2);
    BOOST_CHECK_EQUAL(totalFolders, 1);
    BOOST_CHECK_EQUAL(folders.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_find_all_search_by_caption)
{
    // Arrange
    m_repository->create(createTestItemType("Альфа задача"));
    m_repository->create(createTestItemType("Бета тест"));
    m_repository->create(createTestItemType("Гамма требование"));

    // Act - поиск по слову "задача"
    auto [itemTypes, total] = m_repository->findAll(
        1, 20, std::nullopt, std::nullopt, "задача"
    );

    // Assert
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(itemTypes[0].caption.value(), "Альфа задача");
}

// ============================================================
// Тесты обновления типа элемента
// ============================================================

BOOST_AUTO_TEST_CASE(test_update_item_type_success)
{
    // Arrange
    auto itemType = createTestItemType("Старое название");
    int64_t itemTypeId = m_repository->create(itemType);
    BOOST_REQUIRE_GT(itemTypeId, 0);

    // Act
    dto::ItemType updateData;
    updateData.id = itemTypeId;
    updateData.caption = "Новое название";
    updateData.kind = "folder";
    updateData.defaultContent = "Новое содержимое";

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(itemTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Новое название");
    BOOST_CHECK_EQUAL(found->kind.value(), "folder");
    BOOST_CHECK_EQUAL(found->defaultContent.value(), "Новое содержимое");
}

BOOST_AUTO_TEST_CASE(test_update_item_type_partial)
{
    // Arrange
    auto itemType = createTestItemType("Оригинал", "issue");
    itemType.defaultContent = "Оригинальное содержимое";
    int64_t itemTypeId = m_repository->create(itemType);

    // Act - обновляем только название
    dto::ItemType updateData;
    updateData.id = itemTypeId;
    updateData.caption = "Обновленное название";

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(itemTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленное название");
    BOOST_CHECK_EQUAL(found->kind.value(), "issue"); // Не изменилось
    BOOST_CHECK_EQUAL(found->defaultContent.value(), "Оригинальное содержимое"); // Не изменилось
}

BOOST_AUTO_TEST_CASE(test_update_item_type_invalid_kind_fails)
{
    // Arrange
    auto itemType = createTestItemType();
    int64_t itemTypeId = m_repository->create(itemType);

    // Act
    dto::ItemType updateData;
    updateData.id = itemTypeId;
    updateData.kind = "invalid_kind";

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_update_item_type_nonexistent)
{
    // Arrange
    dto::ItemType updateData;
    updateData.id = 99999;
    updateData.caption = "Несуществующий тип";

    // Act
    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(!result);
}

// ============================================================
// Тесты удаления типа элемента
// ============================================================

BOOST_AUTO_TEST_CASE(test_remove_item_type_success)
{
    // Arrange
    auto itemType = createTestItemType();
    int64_t itemTypeId = m_repository->create(itemType);

    // Act
    bool result = m_repository->remove(itemTypeId);

    // Assert
    BOOST_CHECK(result);
    BOOST_CHECK(!m_repository->exists(itemTypeId));
}

BOOST_AUTO_TEST_CASE(test_remove_item_type_in_use_fails)
{
    // Arrange
    auto itemType = createTestItemType();
    int64_t itemTypeId = m_repository->create(itemType);

    // Создаем элемент, использующий этот тип
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO Item (itemTypeId, stateId, caption) "
        "VALUES (:itemTypeId, :stateId, :caption)"
    );
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->bindInt64("stateId", m_testDefaultStateId);
    stmt->bindString("caption", "Тестовый элемент");
    stmt->execute();

    // Act - пытаемся удалить используемый тип
    bool result = m_repository->remove(itemTypeId);

    // Assert
    BOOST_CHECK(!result);
    BOOST_CHECK(m_repository->exists(itemTypeId)); // Тип все еще существует
}

BOOST_AUTO_TEST_CASE(test_remove_nonexistent_item_type)
{
    // Act
    bool result = m_repository->remove(99999);

    // Assert
    BOOST_CHECK(!result);
}

// ============================================================
// Тесты вспомогательных методов
// ============================================================

BOOST_AUTO_TEST_CASE(test_exists_true)
{
    // Arrange
    int64_t itemTypeId = m_repository->create(createTestItemType());

    // Act
    bool exists = m_repository->exists(itemTypeId);

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

BOOST_AUTO_TEST_CASE(test_find_by_workflow)
{
    // Arrange
    m_repository->create(createTestItemType("Задача", "issue", m_testWorkflowId));
    m_repository->create(createTestItemType("Папка", "folder", m_testWorkflowId));
    m_repository->create(createTestItemType("Тест", "test-case", m_secondWorkflowId));

    // Act
    auto itemTypes = m_repository->findByWorkflow(m_testWorkflowId);

    // Assert
    BOOST_CHECK_EQUAL(itemTypes.size(), 2);

    // Проверяем, что все типы принадлежат правильному workflow
    for (const auto& it : itemTypes)
    {
        BOOST_CHECK_EQUAL(it.workflowId.value(), m_testWorkflowId);
    }
}

BOOST_AUTO_TEST_CASE(test_find_by_workflow_invalid_id)
{
    // Act
    auto itemTypes = m_repository->findByWorkflow(-1);

    // Assert
    BOOST_CHECK(itemTypes.empty());
}

BOOST_AUTO_TEST_CASE(test_usage_count)
{
    // Arrange
    auto itemType = createTestItemType();
    int64_t itemTypeId = m_repository->create(itemType);

    // Проверяем, что изначально счетчик использования равен 0
    BOOST_CHECK_EQUAL(m_repository->usageCount(itemTypeId), 0);

    // Создаем элемент, использующий тип
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO Item (itemTypeId, stateId, caption) "
        "VALUES (:itemTypeId, :stateId, :caption)"
    );
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->bindInt64("stateId", m_testDefaultStateId);
    stmt->bindString("caption", "Элемент 1");
    stmt->execute();

    // Проверяем, что счетчик увеличился
    BOOST_CHECK_EQUAL(m_repository->usageCount(itemTypeId), 1);

    // Создаем еще один элемент
    stmt->reset();
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->bindInt64("stateId", m_testDefaultStateId);
    stmt->bindString("caption", "Элемент 2");
    stmt->execute();

    // Проверяем, что счетчик учитывает все использования
    BOOST_CHECK_EQUAL(m_repository->usageCount(itemTypeId), 2);
}

BOOST_AUTO_TEST_CASE(test_usage_count_considers_all_relations)
{
    // Arrange
    auto itemType = createTestItemType();
    int64_t itemTypeId = m_repository->create(itemType);

    auto secondItemType = createTestItemType("Второй тип", "folder");
    int64_t secondItemTypeId = m_repository->create(secondItemType);

    auto conn = m_database->connection();

    // Использование в Item
    auto stmt = conn->prepareStatement(
        "INSERT INTO Item (itemTypeId, stateId, caption) "
        "VALUES (:itemTypeId, :stateId, :caption)"
    );
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->bindInt64("stateId", m_testDefaultStateId);
    stmt->bindString("caption", "Элемент");
    stmt->execute();

    // Использование в FieldType
    stmt = conn->prepareStatement(
        "INSERT INTO FieldType (itemTypeId, caption, valueType) "
        "VALUES (:itemTypeId, :caption, :valueType)"
    );
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->bindString("caption", "Поле");
    stmt->bindString("valueType", "String");
    stmt->execute();

    // Использование в LinkType
    stmt = conn->prepareStatement(
        "INSERT INTO LinkType (sourceItemTypeId, destinationItemTypeId, caption) "
        "VALUES (:sourceItemTypeId, :destinationItemTypeId, :caption)"
    );
    stmt->bindInt64("sourceItemTypeId", itemTypeId);
    stmt->bindInt64("destinationItemTypeId", secondItemTypeId);
    stmt->bindString("caption", "Связь");
    stmt->execute();

    // Использование в RuleItemType (нужна таблица Rule)
    conn->execute("CREATE TABLE IF NOT EXISTS Rule (id INTEGER PRIMARY KEY, roleId INTEGER)");
    conn->execute("INSERT INTO Rule (roleId) VALUES (1)");
    int64_t ruleId = conn->lastInsertId();

    stmt = conn->prepareStatement(
        "INSERT INTO RuleItemType (ruleId, itemTypeId) "
        "VALUES (:ruleId, :itemTypeId)"
    );
    stmt->bindInt64("ruleId", ruleId);
    stmt->bindInt64("itemTypeId", itemTypeId);
    stmt->execute();

    // Assert
    BOOST_CHECK_EQUAL(m_repository->usageCount(itemTypeId), 4);
}

// ============================================================
// Интеграционный тест: полный жизненный цикл типа элемента
// ============================================================

BOOST_AUTO_TEST_CASE(test_full_item_type_lifecycle)
{
    // 1. Создание
    auto itemType = createTestItemType("Жизненный цикл типа");
    int64_t itemTypeId = m_repository->create(itemType);
    BOOST_CHECK_GT(itemTypeId, 0);

    // 2. Чтение
    auto found = m_repository->findById(itemTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Жизненный цикл типа");

    // 3. Обновление
    dto::ItemType updateData;
    updateData.id = itemTypeId;
    updateData.caption = "Обновленный тип";
    updateData.kind = "requirement";
    updateData.defaultContent = "Новое содержимое по умолчанию";
    BOOST_CHECK(m_repository->update(updateData));

    // 4. Проверка обновления
    found = m_repository->findById(itemTypeId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленный тип");
    BOOST_CHECK_EQUAL(found->kind.value(), "requirement");
    BOOST_CHECK_EQUAL(found->defaultContent.value(), "Новое содержимое по умолчанию");

    // 5. Проверка в списке
    auto [itemTypes, total] = m_repository->findAll(1, 20);
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(itemTypes[0].id.value(), itemTypeId);

    // 6. Проверка использования
    BOOST_CHECK_EQUAL(m_repository->usageCount(itemTypeId), 0);

    // 7. Удаление
    BOOST_CHECK(m_repository->remove(itemTypeId));
    BOOST_CHECK(!m_repository->exists(itemTypeId));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
