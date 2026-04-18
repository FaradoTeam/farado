#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/project.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ProjectTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Project dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasParentId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
    BOOST_TEST(!dto.hasCreatedAt());
    BOOST_TEST(!dto.hasUpdatedAt());
    BOOST_TEST(!dto.hasIsArchive());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Project dto;

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
    // Проверка поля: parentId
    {
        BOOST_TEST(!dto.hasParentId());

        int64_t testValue =42;
        dto.setParentId(testValue);

        BOOST_TEST(dto.hasParentId());

        BOOST_TEST(dto.parentId().value() == testValue);

        // Проверка clear
        dto.clearParentId();
        BOOST_TEST(!dto.hasParentId());
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
    // Проверка поля: createdAt
    {
        BOOST_TEST(!dto.hasCreatedAt());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setCreatedAt(testValue);

        BOOST_TEST(dto.hasCreatedAt());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.createdAt().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearCreatedAt();
        BOOST_TEST(!dto.hasCreatedAt());
    }
    // Проверка поля: updatedAt
    {
        BOOST_TEST(!dto.hasUpdatedAt());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setUpdatedAt(testValue);

        BOOST_TEST(dto.hasUpdatedAt());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.updatedAt().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearUpdatedAt();
        BOOST_TEST(!dto.hasUpdatedAt());
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
    Project dto;

    // Поле: id
    dto.setId(42);
    // Поле: parentId
    dto.setParentId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");
    // Поле: createdAt
    dto.setCreatedAt(secondsToTimePoint(1640995200));
    // Поле: updatedAt
    dto.setUpdatedAt(secondsToTimePoint(1640995200));
    // Поле: isArchive
    dto.setIsArchive(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("parent_id"));
    BOOST_TEST(json["parent_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("description"));
    BOOST_TEST(json["description"].get<std::string>() == "test_description");
    BOOST_TEST(json.contains("created_at"));
    BOOST_TEST(json["created_at"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("updated_at"));
    BOOST_TEST(json["updated_at"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("is_archive"));
    BOOST_TEST(json["is_archive"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["parent_id"] = 42;
    json["caption"] = "test_caption";
    json["description"] = "test_description";
    json["created_at"] = 1640995200;
    json["updated_at"] = 1640995200;
    json["is_archive"] = true;

    Project dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasParentId());
    BOOST_TEST(dto.parentId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
    BOOST_TEST(dto.hasCreatedAt());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.createdAt().value()), 1640995200);
    BOOST_TEST(dto.hasUpdatedAt());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.updatedAt().value()), 1640995200);
    BOOST_TEST(dto.hasIsArchive());
    BOOST_TEST(dto.isArchive().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Project original;

    // Поле: id
    original.setId(42);
    // Поле: parentId
    original.setParentId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");
    // Поле: createdAt
    original.setCreatedAt(secondsToTimePoint(1640995200));
    // Поле: updatedAt
    original.setUpdatedAt(secondsToTimePoint(1640995200));
    // Поле: isArchive
    original.setIsArchive(true);

    nlohmann::json json = original.toJson();
    Project deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Project dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    Project dto1;
    Project dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле caption, чтобы сделать их разными
    dto1.setCaption("different_value");

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    Project dto;

    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()