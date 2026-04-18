#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/edge.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(EdgeTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Edge dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasBeginStateId());
    BOOST_TEST(!dto.hasEndStateId());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Edge dto;

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
    // Проверка поля: beginStateId
    {
        BOOST_TEST(!dto.hasBeginStateId());

        int64_t testValue =42;
        dto.setBeginStateId(testValue);

        BOOST_TEST(dto.hasBeginStateId());

        BOOST_TEST(dto.beginStateId().value() == testValue);

        // Проверка clear
        dto.clearBeginStateId();
        BOOST_TEST(!dto.hasBeginStateId());
    }
    // Проверка поля: endStateId
    {
        BOOST_TEST(!dto.hasEndStateId());

        int64_t testValue =42;
        dto.setEndStateId(testValue);

        BOOST_TEST(dto.hasEndStateId());

        BOOST_TEST(dto.endStateId().value() == testValue);

        // Проверка clear
        dto.clearEndStateId();
        BOOST_TEST(!dto.hasEndStateId());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    Edge dto;

    // Поле: id
    dto.setId(42);
    // Поле: beginStateId
    dto.setBeginStateId(42);
    // Поле: endStateId
    dto.setEndStateId(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("begin_state_id"));
    BOOST_TEST(json["begin_state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("end_state_id"));
    BOOST_TEST(json["end_state_id"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["begin_state_id"] = 42;
    json["end_state_id"] = 42;

    Edge dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasBeginStateId());
    BOOST_TEST(dto.beginStateId().value() == 42);
    BOOST_TEST(dto.hasEndStateId());
    BOOST_TEST(dto.endStateId().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Edge original;

    // Поле: id
    original.setId(42);
    // Поле: beginStateId
    original.setBeginStateId(42);
    // Поле: endStateId
    original.setEndStateId(42);

    nlohmann::json json = original.toJson();
    Edge deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Edge dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setBeginStateId(42);
    dto.setEndStateId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    Edge dto1;
    Edge dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле beginStateId, чтобы сделать их разными
    dto1.setBeginStateId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    Edge dto;

    dto.setBeginStateId(42);
    dto.setEndStateId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()