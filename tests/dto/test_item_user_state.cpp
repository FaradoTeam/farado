#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/item_user_state.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ItemUserStateTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    ItemUserState dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasItemId());
    BOOST_TEST(!dto.hasUserId());
    BOOST_TEST(!dto.hasStateId());
    BOOST_TEST(!dto.hasComment());
    BOOST_TEST(!dto.hasTimestamp());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    ItemUserState dto;

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
    // Проверка поля: comment
    {
        BOOST_TEST(!dto.hasComment());

        std::string testValue ="test_value";
        dto.setComment(testValue);

        BOOST_TEST(dto.hasComment());

        BOOST_TEST(dto.comment().value() == testValue);

        // Проверка clear
        dto.clearComment();
        BOOST_TEST(!dto.hasComment());
    }
    // Проверка поля: timestamp
    {
        BOOST_TEST(!dto.hasTimestamp());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setTimestamp(testValue);

        BOOST_TEST(dto.hasTimestamp());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.timestamp().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearTimestamp();
        BOOST_TEST(!dto.hasTimestamp());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    ItemUserState dto;

    // Поле: id
    dto.setId(42);
    // Поле: itemId
    dto.setItemId(42);
    // Поле: userId
    dto.setUserId(42);
    // Поле: stateId
    dto.setStateId(42);
    // Поле: comment
    dto.setComment("test_comment");
    // Поле: timestamp
    dto.setTimestamp(secondsToTimePoint(1640995200));

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("item_id"));
    BOOST_TEST(json["item_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("user_id"));
    BOOST_TEST(json["user_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("state_id"));
    BOOST_TEST(json["state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("comment"));
    BOOST_TEST(json["comment"].get<std::string>() == "test_comment");
    BOOST_TEST(json.contains("timestamp"));
    BOOST_TEST(json["timestamp"].get<int64_t>() == 1640995200);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["item_id"] = 42;
    json["user_id"] = 42;
    json["state_id"] = 42;
    json["comment"] = "test_comment";
    json["timestamp"] = 1640995200;

    ItemUserState dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasItemId());
    BOOST_TEST(dto.itemId().value() == 42);
    BOOST_TEST(dto.hasUserId());
    BOOST_TEST(dto.userId().value() == 42);
    BOOST_TEST(dto.hasStateId());
    BOOST_TEST(dto.stateId().value() == 42);
    BOOST_TEST(dto.hasComment());
    BOOST_TEST(dto.comment().value() == "test_comment");
    BOOST_TEST(dto.hasTimestamp());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.timestamp().value()), 1640995200);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    ItemUserState original;

    // Поле: id
    original.setId(42);
    // Поле: itemId
    original.setItemId(42);
    // Поле: userId
    original.setUserId(42);
    // Поле: stateId
    original.setStateId(42);
    // Поле: comment
    original.setComment("test_comment");
    // Поле: timestamp
    original.setTimestamp(secondsToTimePoint(1640995200));

    nlohmann::json json = original.toJson();
    ItemUserState deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    ItemUserState dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setItemId(42);
    dto.setUserId(42);
    dto.setStateId(42);
    dto.setTimestamp(secondsToTimePoint(1640995200));

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    ItemUserState dto1;
    ItemUserState dto2;

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
    ItemUserState dto;

    dto.setItemId(42);
    dto.setUserId(42);
    dto.setStateId(42);
    dto.setTimestamp(secondsToTimePoint(1640995200));

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()