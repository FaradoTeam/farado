#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/rule_state.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(RuleStateTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    RuleState dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasRuleId());
    BOOST_TEST(!dto.hasStateId());
    BOOST_TEST(!dto.hasIsStateAllowed());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    RuleState dto;

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
    // Проверка поля: stateId
    {
        BOOST_TEST(!dto.hasStateId());

        int64_t testValue =42;
        dto.setStateId(testValue);

        BOOST_TEST(dto.hasStateId());

        BOOST_TEST(dto.stateId().value() == testValue);

        // Проверка clear
        dto.clearStateId();
        BOOST_TEST(!dto.hasStateId());
    }
    // Проверка поля: isStateAllowed
    {
        BOOST_TEST(!dto.hasIsStateAllowed());

        bool testValue =true;
        dto.setIsStateAllowed(testValue);

        BOOST_TEST(dto.hasIsStateAllowed());

        BOOST_TEST(dto.isStateAllowed().value() == testValue);

        // Проверка clear
        dto.clearIsStateAllowed();
        BOOST_TEST(!dto.hasIsStateAllowed());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    RuleState dto;

    // Поле: id
    dto.setId(42);
    // Поле: ruleId
    dto.setRuleId(42);
    // Поле: stateId
    dto.setStateId(42);
    // Поле: isStateAllowed
    dto.setIsStateAllowed(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("rule_id"));
    BOOST_TEST(json["rule_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("state_id"));
    BOOST_TEST(json["state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("is_state_allowed"));
    BOOST_TEST(json["is_state_allowed"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["rule_id"] = 42;
    json["state_id"] = 42;
    json["is_state_allowed"] = true;

    RuleState dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasRuleId());
    BOOST_TEST(dto.ruleId().value() == 42);
    BOOST_TEST(dto.hasStateId());
    BOOST_TEST(dto.stateId().value() == 42);
    BOOST_TEST(dto.hasIsStateAllowed());
    BOOST_TEST(dto.isStateAllowed().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    RuleState original;

    // Поле: id
    original.setId(42);
    // Поле: ruleId
    original.setRuleId(42);
    // Поле: stateId
    original.setStateId(42);
    // Поле: isStateAllowed
    original.setIsStateAllowed(true);

    nlohmann::json json = original.toJson();
    RuleState deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    RuleState dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setRuleId(42);
    dto.setStateId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    RuleState dto1;
    RuleState dto2;

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
    RuleState dto;

    dto.setRuleId(42);
    dto.setStateId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()