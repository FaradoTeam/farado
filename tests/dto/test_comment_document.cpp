#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/comment_document.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(CommentDocumentTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    CommentDocument dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasCommentId());
    BOOST_TEST(!dto.hasDocumentId());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    CommentDocument dto;

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
    // Проверка поля: commentId
    {
        BOOST_TEST(!dto.hasCommentId());

        int64_t testValue =42;
        dto.setCommentId(testValue);

        BOOST_TEST(dto.hasCommentId());

        BOOST_TEST(dto.commentId().value() == testValue);

        // Проверка clear
        dto.clearCommentId();
        BOOST_TEST(!dto.hasCommentId());
    }
    // Проверка поля: documentId
    {
        BOOST_TEST(!dto.hasDocumentId());

        int64_t testValue =42;
        dto.setDocumentId(testValue);

        BOOST_TEST(dto.hasDocumentId());

        BOOST_TEST(dto.documentId().value() == testValue);

        // Проверка clear
        dto.clearDocumentId();
        BOOST_TEST(!dto.hasDocumentId());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    CommentDocument dto;

    // Поле: id
    dto.setId(42);
    // Поле: commentId
    dto.setCommentId(42);
    // Поле: documentId
    dto.setDocumentId(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("comment_id"));
    BOOST_TEST(json["comment_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("document_id"));
    BOOST_TEST(json["document_id"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["comment_id"] = 42;
    json["document_id"] = 42;

    CommentDocument dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasCommentId());
    BOOST_TEST(dto.commentId().value() == 42);
    BOOST_TEST(dto.hasDocumentId());
    BOOST_TEST(dto.documentId().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    CommentDocument original;

    // Поле: id
    original.setId(42);
    // Поле: commentId
    original.setCommentId(42);
    // Поле: documentId
    original.setDocumentId(42);

    nlohmann::json json = original.toJson();
    CommentDocument deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    CommentDocument dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setCommentId(42);
    dto.setDocumentId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    CommentDocument dto1;
    CommentDocument dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле commentId, чтобы сделать их разными
    dto1.setCommentId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    CommentDocument dto;

    dto.setCommentId(42);
    dto.setDocumentId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()