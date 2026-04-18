#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/rule_project.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(RuleProjectTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    RuleProject dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasRuleId());
    BOOST_TEST(!dto.hasProjectId());
    BOOST_TEST(!dto.hasIsReader());
    BOOST_TEST(!dto.hasIsWriter());
    BOOST_TEST(!dto.hasIsProjectEditor());
    BOOST_TEST(!dto.hasIsPhaseEditor());
    BOOST_TEST(!dto.hasIsBoardEditor());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    RuleProject dto;

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
    // Проверка поля: ruleId
    {
        BOOST_TEST(!dto.hasRuleId());

        int64_t testValue =42;
        dto.setRuleId(testValue);

        BOOST_TEST(dto.hasRuleId());

        BOOST_TEST(dto.ruleId().value() == testValue);

        // Проверка clear
        dto.clearRuleId();
        BOOST_TEST(!dto.hasRuleId());
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
    // Проверка поля: isReader
    {
        BOOST_TEST(!dto.hasIsReader());

        bool testValue =true;
        dto.setIsReader(testValue);

        BOOST_TEST(dto.hasIsReader());

        BOOST_TEST(dto.isReader().value() == testValue);

        // Проверка clear
        dto.clearIsReader();
        BOOST_TEST(!dto.hasIsReader());
    }
    // Проверка поля: isWriter
    {
        BOOST_TEST(!dto.hasIsWriter());

        bool testValue =true;
        dto.setIsWriter(testValue);

        BOOST_TEST(dto.hasIsWriter());

        BOOST_TEST(dto.isWriter().value() == testValue);

        // Проверка clear
        dto.clearIsWriter();
        BOOST_TEST(!dto.hasIsWriter());
    }
    // Проверка поля: isProjectEditor
    {
        BOOST_TEST(!dto.hasIsProjectEditor());

        bool testValue =true;
        dto.setIsProjectEditor(testValue);

        BOOST_TEST(dto.hasIsProjectEditor());

        BOOST_TEST(dto.isProjectEditor().value() == testValue);

        // Проверка clear
        dto.clearIsProjectEditor();
        BOOST_TEST(!dto.hasIsProjectEditor());
    }
    // Проверка поля: isPhaseEditor
    {
        BOOST_TEST(!dto.hasIsPhaseEditor());

        bool testValue =true;
        dto.setIsPhaseEditor(testValue);

        BOOST_TEST(dto.hasIsPhaseEditor());

        BOOST_TEST(dto.isPhaseEditor().value() == testValue);

        // Проверка clear
        dto.clearIsPhaseEditor();
        BOOST_TEST(!dto.hasIsPhaseEditor());
    }
    // Проверка поля: isBoardEditor
    {
        BOOST_TEST(!dto.hasIsBoardEditor());

        bool testValue =true;
        dto.setIsBoardEditor(testValue);

        BOOST_TEST(dto.hasIsBoardEditor());

        BOOST_TEST(dto.isBoardEditor().value() == testValue);

        // Проверка clear
        dto.clearIsBoardEditor();
        BOOST_TEST(!dto.hasIsBoardEditor());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    RuleProject dto;

    // Поле: id
    dto.setId(42);
    // Поле: ruleId
    dto.setRuleId(42);
    // Поле: projectId
    dto.setProjectId(42);
    // Поле: isReader
    dto.setIsReader(true);
    // Поле: isWriter
    dto.setIsWriter(true);
    // Поле: isProjectEditor
    dto.setIsProjectEditor(true);
    // Поле: isPhaseEditor
    dto.setIsPhaseEditor(true);
    // Поле: isBoardEditor
    dto.setIsBoardEditor(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("rule_id"));
    BOOST_TEST(json["rule_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("project_id"));
    BOOST_TEST(json["project_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("is_reader"));
    BOOST_TEST(json["is_reader"].get<bool>() == true);
    BOOST_TEST(json.contains("is_writer"));
    BOOST_TEST(json["is_writer"].get<bool>() == true);
    BOOST_TEST(json.contains("is_project_editor"));
    BOOST_TEST(json["is_project_editor"].get<bool>() == true);
    BOOST_TEST(json.contains("is_phase_editor"));
    BOOST_TEST(json["is_phase_editor"].get<bool>() == true);
    BOOST_TEST(json.contains("is_board_editor"));
    BOOST_TEST(json["is_board_editor"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["rule_id"] = 42;
    json["project_id"] = 42;
    json["is_reader"] = true;
    json["is_writer"] = true;
    json["is_project_editor"] = true;
    json["is_phase_editor"] = true;
    json["is_board_editor"] = true;

    RuleProject dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasRuleId());
    BOOST_TEST(dto.ruleId().value() == 42);
    BOOST_TEST(dto.hasProjectId());
    BOOST_TEST(dto.projectId().value() == 42);
    BOOST_TEST(dto.hasIsReader());
    BOOST_TEST(dto.isReader().value() == true);
    BOOST_TEST(dto.hasIsWriter());
    BOOST_TEST(dto.isWriter().value() == true);
    BOOST_TEST(dto.hasIsProjectEditor());
    BOOST_TEST(dto.isProjectEditor().value() == true);
    BOOST_TEST(dto.hasIsPhaseEditor());
    BOOST_TEST(dto.isPhaseEditor().value() == true);
    BOOST_TEST(dto.hasIsBoardEditor());
    BOOST_TEST(dto.isBoardEditor().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    RuleProject original;

    // Поле: id
    original.setId(42);
    // Поле: ruleId
    original.setRuleId(42);
    // Поле: projectId
    original.setProjectId(42);
    // Поле: isReader
    original.setIsReader(true);
    // Поле: isWriter
    original.setIsWriter(true);
    // Поле: isProjectEditor
    original.setIsProjectEditor(true);
    // Поле: isPhaseEditor
    original.setIsPhaseEditor(true);
    // Поле: isBoardEditor
    original.setIsBoardEditor(true);

    nlohmann::json json = original.toJson();
    RuleProject deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    RuleProject dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setRuleId(42);
    dto.setProjectId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    RuleProject dto1;
    RuleProject dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле ruleId, чтобы сделать их разными
    dto1.setRuleId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    RuleProject dto;

    dto.setRuleId(42);
    dto.setProjectId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()