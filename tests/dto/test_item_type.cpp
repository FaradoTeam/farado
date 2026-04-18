#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/item_type.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ItemTypeTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    ItemType dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasWorkflowId());
    BOOST_TEST(!dto.hasDefaultStateId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasKind());
    BOOST_TEST(!dto.hasDefaultContent());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    ItemType dto;

    // Проверка поля: id
    {
        BOOST_TEST(!dto.hasId());

        int64_t testValue =42;
        dto.setId(testValue);

        BOOST_TEST(dto.hasId());

        BOOST_TEST(dto.id().value() == testValue);

        // Проверка clear
        dto.clearId();
        BOOST_TEST(!dto.hasId());
    }
    // Проверка поля: workflowId
    {
        BOOST_TEST(!dto.hasWorkflowId());

        int64_t testValue =42;
        dto.setWorkflowId(testValue);

        BOOST_TEST(dto.hasWorkflowId());

        BOOST_TEST(dto.workflowId().value() == testValue);

        // Проверка clear
        dto.clearWorkflowId();
        BOOST_TEST(!dto.hasWorkflowId());
    }
    // Проверка поля: defaultStateId
    {
        BOOST_TEST(!dto.hasDefaultStateId());

        int64_t testValue =42;
        dto.setDefaultStateId(testValue);

        BOOST_TEST(dto.hasDefaultStateId());

        BOOST_TEST(dto.defaultStateId().value() == testValue);

        // Проверка clear
        dto.clearDefaultStateId();
        BOOST_TEST(!dto.hasDefaultStateId());
    }
    // Проверка поля: caption
    {
        BOOST_TEST(!dto.hasCaption());

        std::string testValue ="test_value";
        dto.setCaption(testValue);

        BOOST_TEST(dto.hasCaption());

        BOOST_TEST(dto.caption().value() == testValue);

        // Проверка clear
        dto.clearCaption();
        BOOST_TEST(!dto.hasCaption());
    }
    // Проверка поля: kind
    {
        BOOST_TEST(!dto.hasKind());

        std::string testValue ="test_value";
        dto.setKind(testValue);

        BOOST_TEST(dto.hasKind());

        BOOST_TEST(dto.kind().value() == testValue);

        // Проверка clear
        dto.clearKind();
        BOOST_TEST(!dto.hasKind());
    }
    // Проверка поля: defaultContent
    {
        BOOST_TEST(!dto.hasDefaultContent());

        std::string testValue ="test_value";
        dto.setDefaultContent(testValue);

        BOOST_TEST(dto.hasDefaultContent());

        BOOST_TEST(dto.defaultContent().value() == testValue);

        // Проверка clear
        dto.clearDefaultContent();
        BOOST_TEST(!dto.hasDefaultContent());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    ItemType dto;

    // Поле: id
    dto.setId(42);
    // Поле: workflowId
    dto.setWorkflowId(42);
    // Поле: defaultStateId
    dto.setDefaultStateId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: kind
    dto.setKind("test_kind");
    // Поле: defaultContent
    dto.setDefaultContent("test_default_content");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("workflow_id"));
    BOOST_TEST(json["workflow_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("default_state_id"));
    BOOST_TEST(json["default_state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("kind"));
    BOOST_TEST(json["kind"].get<std::string>() == "test_kind");
    BOOST_TEST(json.contains("default_content"));
    BOOST_TEST(json["default_content"].get<std::string>() == "test_default_content");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["workflow_id"] = 42;
    json["default_state_id"] = 42;
    json["caption"] = "test_caption";
    json["kind"] = "test_kind";
    json["default_content"] = "test_default_content";

    ItemType dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasWorkflowId());
    BOOST_TEST(dto.workflowId().value() == 42);
    BOOST_TEST(dto.hasDefaultStateId());
    BOOST_TEST(dto.defaultStateId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasKind());
    BOOST_TEST(dto.kind().value() == "test_kind");
    BOOST_TEST(dto.hasDefaultContent());
    BOOST_TEST(dto.defaultContent().value() == "test_default_content");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    ItemType original;

    // Поле: id
    original.setId(42);
    // Поле: workflowId
    original.setWorkflowId(42);
    // Поле: defaultStateId
    original.setDefaultStateId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: kind
    original.setKind("test_kind");
    // Поле: defaultContent
    original.setDefaultContent("test_default_content");

    nlohmann::json json = original.toJson();
    ItemType deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    ItemType dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setWorkflowId(42);
    dto.setDefaultStateId(42);
    dto.setCaption("test_caption");
    dto.setKind("test_kind");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    ItemType dto1;
    ItemType dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле workflowId, чтобы сделать их разными
    dto1.setWorkflowId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    ItemType dto;

    dto.setWorkflowId(42);
    dto.setDefaultStateId(42);
    dto.setCaption("test_value");
    dto.setKind("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()