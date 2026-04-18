#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/document.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(DocumentTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Document dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
    BOOST_TEST(!dto.hasPath());
    BOOST_TEST(!dto.hasFilename());
    BOOST_TEST(!dto.hasSize());
    BOOST_TEST(!dto.hasMimeType());
    BOOST_TEST(!dto.hasUploadedAt());
    BOOST_TEST(!dto.hasUploadedByUserId());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Document dto;

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
    // Проверка поля: path
    {
        BOOST_TEST(!dto.hasPath());

        std::string testValue ="test_value";
        dto.setPath(testValue);

        BOOST_TEST(dto.hasPath());

        BOOST_TEST(dto.path().value() == testValue);

        // Проверка clear
        dto.clearPath();
        BOOST_TEST(!dto.hasPath());
    }
    // Проверка поля: filename
    {
        BOOST_TEST(!dto.hasFilename());

        std::string testValue ="test_value";
        dto.setFilename(testValue);

        BOOST_TEST(dto.hasFilename());

        BOOST_TEST(dto.filename().value() == testValue);

        // Проверка clear
        dto.clearFilename();
        BOOST_TEST(!dto.hasFilename());
    }
    // Проверка поля: size
    {
        BOOST_TEST(!dto.hasSize());

        int64_t testValue =42;
        dto.setSize(testValue);

        BOOST_TEST(dto.hasSize());

        BOOST_TEST(dto.size().value() == testValue);

        // Проверка clear
        dto.clearSize();
        BOOST_TEST(!dto.hasSize());
    }
    // Проверка поля: mimeType
    {
        BOOST_TEST(!dto.hasMimeType());

        std::string testValue ="test_value";
        dto.setMimeType(testValue);

        BOOST_TEST(dto.hasMimeType());

        BOOST_TEST(dto.mimeType().value() == testValue);

        // Проверка clear
        dto.clearMimeType();
        BOOST_TEST(!dto.hasMimeType());
    }
    // Проверка поля: uploadedAt
    {
        BOOST_TEST(!dto.hasUploadedAt());

        std::chrono::system_clock::time_point testValue =secondsToTimePoint(1640995200);
        dto.setUploadedAt(testValue);

        BOOST_TEST(dto.hasUploadedAt());

        BOOST_CHECK_EQUAL(
            timePointToSeconds(dto.uploadedAt().value()),
            timePointToSeconds(testValue)
        );

        // Проверка clear
        dto.clearUploadedAt();
        BOOST_TEST(!dto.hasUploadedAt());
    }
    // Проверка поля: uploadedByUserId
    {
        BOOST_TEST(!dto.hasUploadedByUserId());

        int64_t testValue =42;
        dto.setUploadedByUserId(testValue);

        BOOST_TEST(dto.hasUploadedByUserId());

        BOOST_TEST(dto.uploadedByUserId().value() == testValue);

        // Проверка clear
        dto.clearUploadedByUserId();
        BOOST_TEST(!dto.hasUploadedByUserId());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    Document dto;

    // Поле: id
    dto.setId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");
    // Поле: path
    dto.setPath("test_path");
    // Поле: filename
    dto.setFilename("test_filename");
    // Поле: size
    dto.setSize(42);
    // Поле: mimeType
    dto.setMimeType("test_mime_type");
    // Поле: uploadedAt
    dto.setUploadedAt(secondsToTimePoint(1640995200));
    // Поле: uploadedByUserId
    dto.setUploadedByUserId(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("description"));
    BOOST_TEST(json["description"].get<std::string>() == "test_description");
    BOOST_TEST(json.contains("path"));
    BOOST_TEST(json["path"].get<std::string>() == "test_path");
    BOOST_TEST(json.contains("filename"));
    BOOST_TEST(json["filename"].get<std::string>() == "test_filename");
    BOOST_TEST(json.contains("size"));
    BOOST_TEST(json["size"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("mime_type"));
    BOOST_TEST(json["mime_type"].get<std::string>() == "test_mime_type");
    BOOST_TEST(json.contains("uploaded_at"));
    BOOST_TEST(json["uploaded_at"].get<int64_t>() == 1640995200);
    BOOST_TEST(json.contains("uploaded_by_user_id"));
    BOOST_TEST(json["uploaded_by_user_id"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["caption"] = "test_caption";
    json["description"] = "test_description";
    json["path"] = "test_path";
    json["filename"] = "test_filename";
    json["size"] = 42;
    json["mime_type"] = "test_mime_type";
    json["uploaded_at"] = 1640995200;
    json["uploaded_by_user_id"] = 42;

    Document dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
    BOOST_TEST(dto.hasPath());
    BOOST_TEST(dto.path().value() == "test_path");
    BOOST_TEST(dto.hasFilename());
    BOOST_TEST(dto.filename().value() == "test_filename");
    BOOST_TEST(dto.hasSize());
    BOOST_TEST(dto.size().value() == 42);
    BOOST_TEST(dto.hasMimeType());
    BOOST_TEST(dto.mimeType().value() == "test_mime_type");
    BOOST_TEST(dto.hasUploadedAt());
    BOOST_CHECK_EQUAL(timePointToSeconds(dto.uploadedAt().value()), 1640995200);
    BOOST_TEST(dto.hasUploadedByUserId());
    BOOST_TEST(dto.uploadedByUserId().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Document original;

    // Поле: id
    original.setId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");
    // Поле: path
    original.setPath("test_path");
    // Поле: filename
    original.setFilename("test_filename");
    // Поле: size
    original.setSize(42);
    // Поле: mimeType
    original.setMimeType("test_mime_type");
    // Поле: uploadedAt
    original.setUploadedAt(secondsToTimePoint(1640995200));
    // Поле: uploadedByUserId
    original.setUploadedByUserId(42);

    nlohmann::json json = original.toJson();
    Document deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Document dto;

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
    Document dto1;
    Document dto2;

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
    Document dto;

    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()