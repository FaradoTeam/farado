#include <cstdio>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "common/dto/field_type.h"

#include "repo/sqlite/sqlite_field_type_repository.h"

#include "storage/idatabase.h"
#include "storage/sqlite/sqlite_database.h"

namespace server::test
{

struct FieldTypeRepositoryFixture
{
    FieldTypeRepositoryFixture()
    {
        // Создаем временный файл для тестовой БД
        m_tempDbPath = std::filesystem::temp_directory_path() / "test_field_type_repo.db";

        // Удаляем файл если он существует от предыдущего запуска
        std::remove(m_tempDbPath.c_str());

        // Создаем файловую БД
        db::DatabaseConfig config;
        config["database"] = m_tempDbPath.string();

        m_database = std::make_shared<db::SqliteDatabase>();
        m_database->initialize(config);

        // Используем ОДНО соединение для создания схемы
        auto conn = m_database->connection();

        // Создаем схему для ItemType и FieldType
        conn->execute(R"(
            CREATE TABLE ItemType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                workflowId INTEGER NOT NULL,
                defaultStateId INTEGER,
                caption TEXT NOT NULL,
                kind TEXT NOT NULL,
                defaultContent TEXT
            )
        )");

        conn->execute(R"(
            CREATE TABLE FieldType (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                itemTypeId INTEGER NOT NULL,
                caption TEXT NOT NULL,
                description TEXT,
                valueType TEXT NOT NULL CHECK (valueType IN (
                    'String', 'MarkdownText', 'Integer', 'Float', 'Bool',
                    'DateTime', 'Date', 'Uri', 'Select', 'SelectMulti',
                    'ItemId', 'UserId', 'ProjectId', 'PhaseId',
                    'ItemsIds', 'UsersIds', 'ProjectsIds', 'PhasesIds'
                )),
                isBoardVisible INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (itemTypeId) REFERENCES ItemType(id) ON DELETE CASCADE
            )
        )");

        // Создаем связанные таблицы для проверки usageCount
        conn->execute(R"(
            CREATE TABLE ItemField (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                itemId INTEGER NOT NULL,
                fieldTypeId INTEGER NOT NULL,
                value TEXT,
                searchValue TEXT,
                FOREIGN KEY (fieldTypeId) REFERENCES FieldType(id) ON DELETE CASCADE
            )
        )");

        conn->execute(R"(
            CREATE TABLE FieldTypePossibleValue (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                fieldTypeId INTEGER NOT NULL,
                value TEXT NOT NULL,
                FOREIGN KEY (fieldTypeId) REFERENCES FieldType(id) ON DELETE CASCADE
            )
        )");

        // Создаем тестовый ItemType
        conn->execute(R"(
            INSERT INTO ItemType (workflowId, caption, kind)
            VALUES (1, 'Задача', 'issue')
        )");
        m_testItemTypeId = conn->lastInsertId();

        // Создаем второй ItemType для тестирования фильтрации
        conn->execute(R"(
            INSERT INTO ItemType (workflowId, caption, kind)
            VALUES (1, 'Требование', 'requirement')
        )");
        m_secondItemTypeId = conn->lastInsertId();

        // Создаем репозиторий
        m_repository = std::make_unique<repositories::SqliteFieldTypeRepository>(m_database);
    }

    /**
     * @brief Создает тестовый тип поля.
     */
    dto::FieldType createTestFieldType(
        const std::string& caption = "Приоритет",
        const std::string& valueType = "Select",
        std::optional<int64_t> itemTypeId = std::nullopt
    )
    {
        dto::FieldType fieldType;
        fieldType.caption = caption;
        fieldType.valueType = valueType;
        fieldType.itemTypeId = itemTypeId.has_value() ? itemTypeId.value() : m_testItemTypeId;
        fieldType.description = "Описание тестового поля";
        fieldType.isBoardVisible = true;
        return fieldType;
    }

    ~FieldTypeRepositoryFixture()
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
    std::unique_ptr<repositories::SqliteFieldTypeRepository> m_repository;
    int64_t m_testItemTypeId = 0;
    int64_t m_secondItemTypeId = 0;
};

BOOST_FIXTURE_TEST_SUITE(SqliteFieldTypeRepositoryTests, FieldTypeRepositoryFixture)

// ============================================================
// Тесты создания типа поля
// ============================================================

BOOST_AUTO_TEST_CASE(test_create_field_type_success)
{
    // Arrange
    auto fieldType = createTestFieldType();

    // Act
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Assert
    BOOST_CHECK_GT(fieldTypeId, 0);
    BOOST_CHECK(m_repository->exists(fieldTypeId));

    auto found = m_repository->findById(fieldTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->caption.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Приоритет");
    BOOST_CHECK(found->valueType.has_value());
    BOOST_CHECK_EQUAL(found->valueType.value(), "Select");
    BOOST_CHECK(found->itemTypeId.has_value());
    BOOST_CHECK_EQUAL(found->itemTypeId.value(), m_testItemTypeId);
    BOOST_CHECK(found->isBoardVisible.has_value());
    BOOST_CHECK(found->isBoardVisible.value());
    BOOST_CHECK(found->description.has_value());
    BOOST_CHECK_EQUAL(found->description.value(), "Описание тестового поля");
}

BOOST_AUTO_TEST_CASE(test_create_field_type_without_caption_fails)
{
    // Arrange
    dto::FieldType fieldType;
    fieldType.itemTypeId = m_testItemTypeId;
    fieldType.valueType = "String";

    // Act
    int64_t result = m_repository->create(fieldType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_field_type_invalid_value_type_fails)
{
    // Arrange
    auto fieldType = createTestFieldType();
    fieldType.valueType = "InvalidType";

    // Act
    int64_t result = m_repository->create(fieldType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_field_type_all_valid_value_types)
{
    // Arrange
    std::vector<std::string> validValueTypes = {
        "String", "MarkdownText", "Integer", "Float", "Bool",
        "DateTime", "Date", "Uri", "Select", "SelectMulti",
        "ItemId", "UserId", "ProjectId", "PhaseId",
        "ItemsIds", "UsersIds", "ProjectsIds", "PhasesIds"
    };

    for (const auto& valueType : validValueTypes)
    {
        auto fieldType = createTestFieldType("Field " + valueType, valueType);

        // Act
        int64_t id = m_repository->create(fieldType);

        // Assert
        BOOST_CHECK_GT(id, 0);
        BOOST_CHECK(m_repository->exists(id));

        auto found = m_repository->findById(id);
        BOOST_REQUIRE(found.has_value());
        BOOST_CHECK_EQUAL(found->valueType.value(), valueType);
    }
}

BOOST_AUTO_TEST_CASE(test_create_field_type_without_item_type_fails)
{
    // Arrange
    dto::FieldType fieldType;
    fieldType.caption = "Поле без типа элемента";
    fieldType.valueType = "String";

    // Act
    int64_t result = m_repository->create(fieldType);

    // Assert
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE(test_create_field_type_not_board_visible_by_default)
{
    // Arrange
    auto fieldType = createTestFieldType();
    fieldType.isBoardVisible = false;

    // Act
    int64_t id = m_repository->create(fieldType);

    // Assert
    auto found = m_repository->findById(id);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK(found->isBoardVisible.has_value());
    BOOST_CHECK(!found->isBoardVisible.value());
}

// ============================================================
// Тесты поиска типа поля
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_by_id_success)
{
    // Arrange
    auto fieldType = createTestFieldType("Поле для поиска");
    int64_t fieldTypeId = m_repository->create(fieldType);
    BOOST_REQUIRE_GT(fieldTypeId, 0);

    // Act
    auto found = m_repository->findById(fieldTypeId);

    // Assert
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->id.value(), fieldTypeId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Поле для поиска");
    BOOST_CHECK_EQUAL(found->valueType.value(), "Select");
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
// Тесты получения списка типов полей с пагинацией
// ============================================================

BOOST_AUTO_TEST_CASE(test_find_all_empty)
{
    // Act
    auto [fieldTypes, total] = m_repository->findAll(1, 20);

    // Assert
    BOOST_CHECK_EQUAL(total, 0);
    BOOST_CHECK(fieldTypes.empty());
}

BOOST_AUTO_TEST_CASE(test_find_all_with_pagination)
{
    // Arrange - создаем 15 типов полей
    for (int i = 1; i <= 15; ++i)
    {
        m_repository->create(createTestFieldType("Поле " + std::to_string(i)));
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

BOOST_AUTO_TEST_CASE(test_find_all_by_item_type)
{
    // Arrange
    m_repository->create(createTestFieldType("Поле 1", "String", m_testItemTypeId));
    m_repository->create(createTestFieldType("Поле 2", "Integer", m_testItemTypeId));
    m_repository->create(createTestFieldType("Поле 3", "Bool", m_secondItemTypeId));

    // Act - фильтр по первому ItemType
    auto [fields1, total1] = m_repository->findAll(1, 20, m_testItemTypeId);

    // Assert
    BOOST_CHECK_EQUAL(total1, 2);
    BOOST_CHECK_EQUAL(fields1.size(), 2);

    // Act - фильтр по второму ItemType
    auto [fields2, total2] = m_repository->findAll(1, 20, m_secondItemTypeId);
    BOOST_CHECK_EQUAL(total2, 1);
    BOOST_CHECK_EQUAL(fields2.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_find_all_by_value_type)
{
    // Arrange
    m_repository->create(createTestFieldType("Строка 1", "String"));
    m_repository->create(createTestFieldType("Строка 2", "String"));
    m_repository->create(createTestFieldType("Число 1", "Integer"));

    // Act - фильтр по valueType
    auto [strings, totalStrings] = m_repository->findAll(
        1, 20, std::nullopt, std::string("String")
    );
    auto [integers, totalIntegers] = m_repository->findAll(
        1, 20, std::nullopt, std::string("Integer")
    );

    // Assert
    BOOST_CHECK_EQUAL(totalStrings, 2);
    BOOST_CHECK_EQUAL(strings.size(), 2);
    BOOST_CHECK_EQUAL(totalIntegers, 1);
    BOOST_CHECK_EQUAL(integers.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_find_all_search_by_caption)
{
    // Arrange
    m_repository->create(createTestFieldType("Альфа приоритет"));
    m_repository->create(createTestFieldType("Бета статус"));
    m_repository->create(createTestFieldType("Гамма сложность"));

    // Act - поиск по слову "приоритет"
    auto [fieldTypes, total] = m_repository->findAll(
        1, 20, std::nullopt, std::nullopt, "приоритет"
    );

    // Assert
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(fieldTypes[0].caption.value(), "Альфа приоритет");
}

// ============================================================
// Тесты обновления типа поля
// ============================================================

BOOST_AUTO_TEST_CASE(test_update_field_type_success)
{
    // Arrange
    auto fieldType = createTestFieldType("Старое поле");
    int64_t fieldTypeId = m_repository->create(fieldType);
    BOOST_REQUIRE_GT(fieldTypeId, 0);

    // Act
    dto::FieldType updateData;
    updateData.id = fieldTypeId;
    updateData.caption = "Новое поле";
    updateData.valueType = "Integer";
    updateData.description = "Новое описание";
    updateData.isBoardVisible = false;

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(fieldTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Новое поле");
    BOOST_CHECK_EQUAL(found->valueType.value(), "Integer");
    BOOST_CHECK_EQUAL(found->description.value(), "Новое описание");
    BOOST_CHECK(!found->isBoardVisible.value());
}

BOOST_AUTO_TEST_CASE(test_update_field_type_partial)
{
    // Arrange
    auto fieldType = createTestFieldType("Оригинал", "String");
    fieldType.description = "Оригинальное описание";
    fieldType.isBoardVisible = true;
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Act - обновляем только caption и isBoardVisible
    dto::FieldType updateData;
    updateData.id = fieldTypeId;
    updateData.caption = "Обновленное поле";
    updateData.isBoardVisible = false;

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(result);

    auto found = m_repository->findById(fieldTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленное поле");
    BOOST_CHECK_EQUAL(found->valueType.value(), "String"); // Не изменилось
    BOOST_CHECK_EQUAL(found->description.value(), "Оригинальное описание"); // Не изменилось
    BOOST_CHECK(!found->isBoardVisible.value()); // Изменилось
}

BOOST_AUTO_TEST_CASE(test_update_field_type_invalid_value_type_fails)
{
    // Arrange
    auto fieldType = createTestFieldType();
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Act
    dto::FieldType updateData;
    updateData.id = fieldTypeId;
    updateData.valueType = "InvalidType";

    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_update_field_type_nonexistent)
{
    // Arrange
    dto::FieldType updateData;
    updateData.id = 99999;
    updateData.caption = "Несуществующее поле";

    // Act
    bool result = m_repository->update(updateData);

    // Assert
    BOOST_CHECK(!result);
}

// ============================================================
// Тесты удаления типа поля
// ============================================================

BOOST_AUTO_TEST_CASE(test_remove_field_type_success)
{
    // Arrange
    auto fieldType = createTestFieldType();
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Act
    bool result = m_repository->remove(fieldTypeId);

    // Assert
    BOOST_CHECK(result);
    BOOST_CHECK(!m_repository->exists(fieldTypeId));
}

BOOST_AUTO_TEST_CASE(test_remove_field_type_in_use_fails)
{
    // Arrange
    auto fieldType = createTestFieldType();
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Создаем значение поля, использующее этот тип
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO ItemField (itemId, fieldTypeId, value) "
        "VALUES (:itemId, :fieldTypeId, :value)"
    );
    stmt->bindInt64("itemId", 1);
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Тестовое значение");
    stmt->execute();

    // Act - пытаемся удалить используемый тип
    bool result = m_repository->remove(fieldTypeId);

    // Assert
    BOOST_CHECK(!result);
    BOOST_CHECK(m_repository->exists(fieldTypeId)); // Тип все еще существует
}

BOOST_AUTO_TEST_CASE(test_remove_nonexistent_field_type)
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
    int64_t fieldTypeId = m_repository->create(createTestFieldType());

    // Act
    bool exists = m_repository->exists(fieldTypeId);

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

BOOST_AUTO_TEST_CASE(test_find_by_item_type)
{
    // Arrange
    m_repository->create(createTestFieldType("Приоритет", "Select", m_testItemTypeId));
    m_repository->create(createTestFieldType("Статус", "String", m_testItemTypeId));
    m_repository->create(createTestFieldType("Версия", "String", m_secondItemTypeId));

    // Act
    auto fields = m_repository->findByItemType(m_testItemTypeId);

    // Assert
    BOOST_CHECK_EQUAL(fields.size(), 2);

    // Проверяем, что все поля принадлежат правильному ItemType
    for (const auto& field : fields)
    {
        BOOST_CHECK_EQUAL(field.itemTypeId.value(), m_testItemTypeId);
    }
}

BOOST_AUTO_TEST_CASE(test_find_by_item_type_invalid_id)
{
    // Act
    auto fields = m_repository->findByItemType(-1);

    // Assert
    BOOST_CHECK(fields.empty());
}

BOOST_AUTO_TEST_CASE(test_usage_count)
{
    // Arrange
    auto fieldType = createTestFieldType();
    int64_t fieldTypeId = m_repository->create(fieldType);

    // Проверяем, что изначально счетчик использования равен 0
    BOOST_CHECK_EQUAL(m_repository->usageCount(fieldTypeId), 0);

    // Создаем значение поля, использующее тип
    auto conn = m_database->connection();
    auto stmt = conn->prepareStatement(
        "INSERT INTO ItemField (itemId, fieldTypeId, value) "
        "VALUES (:itemId, :fieldTypeId, :value)"
    );
    stmt->bindInt64("itemId", 1);
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Значение 1");
    stmt->execute();

    // Проверяем, что счетчик увеличился
    BOOST_CHECK_EQUAL(m_repository->usageCount(fieldTypeId), 1);

    // Создаем еще одно значение
    stmt->reset();
    stmt->bindInt64("itemId", 2);
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Значение 2");
    stmt->execute();

    // Проверяем, что счетчик учитывает все использования
    BOOST_CHECK_EQUAL(m_repository->usageCount(fieldTypeId), 2);
}

BOOST_AUTO_TEST_CASE(test_usage_count_considers_all_relations)
{
    // Arrange
    auto fieldType = createTestFieldType("Select поле", "Select");
    int64_t fieldTypeId = m_repository->create(fieldType);

    auto conn = m_database->connection();

    // Использование в ItemField
    auto stmt = conn->prepareStatement(
        "INSERT INTO ItemField (itemId, fieldTypeId, value) "
        "VALUES (:itemId, :fieldTypeId, :value)"
    );
    stmt->bindInt64("itemId", 1);
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Значение поля");
    stmt->execute();

    // Использование в FieldTypePossibleValue
    stmt = conn->prepareStatement(
        "INSERT INTO FieldTypePossibleValue (fieldTypeId, value) "
        "VALUES (:fieldTypeId, :value)"
    );
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Высокий");
    stmt->execute();

    stmt->reset();
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Средний");
    stmt->execute();

    stmt->reset();
    stmt->bindInt64("fieldTypeId", fieldTypeId);
    stmt->bindString("value", "Низкий");
    stmt->execute();

    // Assert
    BOOST_CHECK_EQUAL(m_repository->usageCount(fieldTypeId), 4);
}

// ============================================================
// Интеграционный тест: полный жизненный цикл типа поля
// ============================================================

BOOST_AUTO_TEST_CASE(test_full_field_type_lifecycle)
{
    // 1. Создание
    auto fieldType = createTestFieldType("Жизненный цикл поля", "MarkdownText");
    int64_t fieldTypeId = m_repository->create(fieldType);
    BOOST_CHECK_GT(fieldTypeId, 0);

    // 2. Чтение
    auto found = m_repository->findById(fieldTypeId);
    BOOST_REQUIRE(found.has_value());
    BOOST_CHECK_EQUAL(found->caption.value(), "Жизненный цикл поля");
    BOOST_CHECK_EQUAL(found->valueType.value(), "MarkdownText");

    // 3. Обновление
    dto::FieldType updateData;
    updateData.id = fieldTypeId;
    updateData.caption = "Обновленное поле";
    updateData.valueType = "String";
    updateData.description = "Обновленное описание поля";
    updateData.isBoardVisible = false;
    BOOST_CHECK(m_repository->update(updateData));

    // 4. Проверка обновления
    found = m_repository->findById(fieldTypeId);
    BOOST_CHECK_EQUAL(found->caption.value(), "Обновленное поле");
    BOOST_CHECK_EQUAL(found->valueType.value(), "String");
    BOOST_CHECK_EQUAL(found->description.value(), "Обновленное описание поля");
    BOOST_CHECK(!found->isBoardVisible.value());

    // 5. Проверка в списке
    auto [fieldTypes, total] = m_repository->findAll(1, 20);
    BOOST_CHECK_EQUAL(total, 1);
    BOOST_CHECK_EQUAL(fieldTypes[0].id.value(), fieldTypeId);

    // 6. Проверка использования
    BOOST_CHECK_EQUAL(m_repository->usageCount(fieldTypeId), 0);

    // 7. Удаление
    BOOST_CHECK(m_repository->remove(fieldTypeId));
    BOOST_CHECK(!m_repository->exists(fieldTypeId));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace server::test
