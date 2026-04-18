#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/user_todo.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(UserTodoTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    UserTodo dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasItemId());
    BOOST_TEST(!dto.hasUserId());
    BOOST_TEST(!dto.hasStartDate());
    BOOST_TEST(!dto.hasEndDate());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    UserTodo dto;

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
    // Проверка поля: itemId
    {
        BOOST_TEST(!dto.hasItemId());

        int64_t testValue =42;
        dto.setItemId(testValue);

        BOOST_TEST(dto.hasItemId());

        BOOST_TEST(dto.itemId().value() == testValue);

        // Проверка clear
        dto.clearItemId();
        BOOST_TEST(!dto.hasItemId());
    }
    // Проверка поля: userId
    {
        BOOST_TEST(!dto.hasUserId());

        int64_t testValue =42;
        dto.setUserId(testValue);

        BOOST_TEST(dto.hasUserId());

        BOOST_TEST(dto.userId().value() == testValue);

        // Проверка clear
        dto.clearUserId();
        BOOST_TEST(!dto.hasUserId());
    }
    // Проверка поля: startDate
    {
        BOOST_TEST(!dto.hasStartDate());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setStartDate(testValue);

        BOOST_TEST(dto.hasStartDate());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.startDate().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearStartDate();
        BOOST_TEST(!dto.hasStartDate());
    }
    // Проверка поля: endDate
    {
        BOOST_TEST(!dto.hasEndDate());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setEndDate(testValue);

        BOOST_TEST(dto.hasEndDate());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.endDate().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearEndDate();
        BOOST_TEST(!dto.hasEndDate());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    UserTodo dto;

    // Поле: id
    dto.setId(42);
    // Поле: itemId
    dto.setItemId(42);
    // Поле: userId
    dto.setUserId(42);
    // Поле: startDate
    dto.setStartDate(secondsToTimePoint(1640995200));
    // Поле: endDate
    dto.setEndDate(secondsToTimePoint(1640995200));

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("item_id"));
    BOOST_TEST(json["item_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("user_id"));
    BOOST_TEST(json["user_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("start_date"));
    BOOST_TEST(json["start_date"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("end_date"));
    BOOST_TEST(json["end_date"].get<int64_t>() == 1640995200);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["item_id"] = 42;
    json["user_id"] = 42;
    json["start_date"] = 1640995200;
    json["end_date"] = 1640995200;

    UserTodo dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasItemId());
    BOOST_TEST(dto.itemId().value() == 42);
    BOOST_TEST(dto.hasUserId());
    BOOST_TEST(dto.userId().value() == 42);
    BOOST_TEST(dto.hasStartDate());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.startDate().value()), 1640995200);
    BOOST_TEST(dto.hasEndDate());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.endDate().value()), 1640995200);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    UserTodo original;

    // Поле: id
    original.setId(42);
    // Поле: itemId
    original.setItemId(42);
    // Поле: userId
    original.setUserId(42);
    // Поле: startDate
    original.setStartDate(secondsToTimePoint(1640995200));
    // Поле: endDate
    original.setEndDate(secondsToTimePoint(1640995200));

    nlohmann::json json = original.toJson();
    UserTodo deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    UserTodo dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setItemId(42);
    dto.setUserId(42);
    dto.setStartDate(secondsToTimePoint(1640995200));
    dto.setEndDate(secondsToTimePoint(1640995200));

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    UserTodo dto1;
    UserTodo dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле itemId, чтобы сделать их разными
    dto1.setItemId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    UserTodo dto;

    dto.setItemId(42);
    dto.setUserId(42);
    dto.setStartDate(secondsToTimePoint(1640995200));
    dto.setEndDate(secondsToTimePoint(1640995200));

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()