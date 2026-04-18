#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/board.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(BoardTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Board dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasWorkflowId());
    BOOST_TEST(!dto.hasProjectId());
    BOOST_TEST(!dto.hasPhaseId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Board dto;

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
    // Проверка поля: projectId
    {
        BOOST_TEST(!dto.hasProjectId());

        int64_t testValue =42;
        dto.setProjectId(testValue);

        BOOST_TEST(dto.hasProjectId());

        BOOST_TEST(dto.projectId().value() == testValue);

        // Проверка clear
        dto.clearProjectId();
        BOOST_TEST(!dto.hasProjectId());
    }
    // Проверка поля: phaseId
    {
        BOOST_TEST(!dto.hasPhaseId());

        int64_t testValue =42;
        dto.setPhaseId(testValue);

        BOOST_TEST(dto.hasPhaseId());

        BOOST_TEST(dto.phaseId().value() == testValue);

        // Проверка clear
        dto.clearPhaseId();
        BOOST_TEST(!dto.hasPhaseId());
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
    // Проверка поля: description
    {
        BOOST_TEST(!dto.hasDescription());

        std::string testValue ="test_value";
        dto.setDescription(testValue);

        BOOST_TEST(dto.hasDescription());

        BOOST_TEST(dto.description().value() == testValue);

        // Проверка clear
        dto.clearDescription();
        BOOST_TEST(!dto.hasDescription());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    Board dto;

    // Поле: id
    dto.setId(42);
    // Поле: workflowId
    dto.setWorkflowId(42);
    // Поле: projectId
    dto.setProjectId(42);
    // Поле: phaseId
    dto.setPhaseId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("workflow_id"));
    BOOST_TEST(json["workflow_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("project_id"));
    BOOST_TEST(json["project_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("phase_id"));
    BOOST_TEST(json["phase_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("description"));
    BOOST_TEST(json["description"].get<std::string>() == "test_description");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["workflow_id"] = 42;
    json["project_id"] = 42;
    json["phase_id"] = 42;
    json["caption"] = "test_caption";
    json["description"] = "test_description";

    Board dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasWorkflowId());
    BOOST_TEST(dto.workflowId().value() == 42);
    BOOST_TEST(dto.hasProjectId());
    BOOST_TEST(dto.projectId().value() == 42);
    BOOST_TEST(dto.hasPhaseId());
    BOOST_TEST(dto.phaseId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Board original;

    // Поле: id
    original.setId(42);
    // Поле: workflowId
    original.setWorkflowId(42);
    // Поле: projectId
    original.setProjectId(42);
    // Поле: phaseId
    original.setPhaseId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");

    nlohmann::json json = original.toJson();
    Board deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Board dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setWorkflowId(42);
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    Board dto1;
    Board dto2;

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
    Board dto;

    dto.setWorkflowId(42);
    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()