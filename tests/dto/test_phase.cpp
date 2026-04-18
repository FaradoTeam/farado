#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/phase.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(PhaseTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Phase dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasProjectId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
    BOOST_TEST(!dto.hasBeginDate());
    BOOST_TEST(!dto.hasEndDate());
    BOOST_TEST(!dto.hasIsArchive());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Phase dto;

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
    // Проверка поля: beginDate
    {
        BOOST_TEST(!dto.hasBeginDate());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setBeginDate(testValue);

        BOOST_TEST(dto.hasBeginDate());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.beginDate().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearBeginDate();
        BOOST_TEST(!dto.hasBeginDate());
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
    // Проверка поля: isArchive
    {
        BOOST_TEST(!dto.hasIsArchive());

        bool testValue =true;
        dto.setIsArchive(testValue);

        BOOST_TEST(dto.hasIsArchive());

        BOOST_TEST(dto.isArchive().value() == testValue);

        // Проверка clear
        dto.clearIsArchive();
        BOOST_TEST(!dto.hasIsArchive());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    Phase dto;

    // Поле: id
    dto.setId(42);
    // Поле: projectId
    dto.setProjectId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");
    // Поле: beginDate
    dto.setBeginDate(secondsToTimePoint(1640995200));
    // Поле: endDate
    dto.setEndDate(secondsToTimePoint(1640995200));
    // Поле: isArchive
    dto.setIsArchive(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("project_id"));
    BOOST_TEST(json["project_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("description"));
    BOOST_TEST(json["description"].get<std::string>() == "test_description");
    BOOST_TEST(json.contains("begin_date"));
    BOOST_TEST(json["begin_date"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("end_date"));
    BOOST_TEST(json["end_date"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("is_archive"));
    BOOST_TEST(json["is_archive"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["project_id"] = 42;
    json["caption"] = "test_caption";
    json["description"] = "test_description";
    json["begin_date"] = 1640995200;
    json["end_date"] = 1640995200;
    json["is_archive"] = true;

    Phase dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasProjectId());
    BOOST_TEST(dto.projectId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
    BOOST_TEST(dto.hasBeginDate());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.beginDate().value()), 1640995200);
    BOOST_TEST(dto.hasEndDate());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.endDate().value()), 1640995200);
    BOOST_TEST(dto.hasIsArchive());
    BOOST_TEST(dto.isArchive().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Phase original;

    // Поле: id
    original.setId(42);
    // Поле: projectId
    original.setProjectId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");
    // Поле: beginDate
    original.setBeginDate(secondsToTimePoint(1640995200));
    // Поле: endDate
    original.setEndDate(secondsToTimePoint(1640995200));
    // Поле: isArchive
    original.setIsArchive(true);

    nlohmann::json json = original.toJson();
    Phase deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Phase dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setProjectId(42);
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    Phase dto1;
    Phase dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле projectId, чтобы сделать их разными
    dto1.setProjectId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    Phase dto;

    dto.setProjectId(42);
    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()