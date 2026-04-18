#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/user_action.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(UserActionTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    UserAction dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasUserId());
    BOOST_TEST(!dto.hasTimestamp());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    UserAction dto;

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
    UserAction dto;

    // Поле: id
    dto.setId(42);
    // Поле: userId
    dto.setUserId(42);
    // Поле: timestamp
    dto.setTimestamp(secondsToTimePoint(1640995200));
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("user_id"));
    BOOST_TEST(json["user_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("timestamp"));
    BOOST_TEST(json["timestamp"].get<int64_t>() == 1640995200);
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
    json["user_id"] = 42;
    json["timestamp"] = 1640995200;
    json["caption"] = "test_caption";
    json["description"] = "test_description";

    UserAction dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasUserId());
    BOOST_TEST(dto.userId().value() == 42);
    BOOST_TEST(dto.hasTimestamp());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.timestamp().value()), 1640995200);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    UserAction original;

    // Поле: id
    original.setId(42);
    // Поле: userId
    original.setUserId(42);
    // Поле: timestamp
    original.setTimestamp(secondsToTimePoint(1640995200));
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");

    nlohmann::json json = original.toJson();
    UserAction deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    UserAction dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setUserId(42);
    dto.setTimestamp(secondsToTimePoint(1640995200));
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    UserAction dto1;
    UserAction dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле userId, чтобы сделать их разными
    dto1.setUserId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    UserAction dto;

    dto.setUserId(42);
    dto.setTimestamp(secondsToTimePoint(1640995200));
    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()