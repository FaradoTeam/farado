#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/field_type.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(FieldTypeTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    FieldType dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasItemTypeId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasDescription());
    BOOST_TEST(!dto.hasValueType());
    BOOST_TEST(!dto.hasIsBoardVisible());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    FieldType dto;

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
    // Проверка поля: itemTypeId
    {
        BOOST_TEST(!dto.hasItemTypeId());

        int64_t testValue =42;
        dto.setItemTypeId(testValue);

        BOOST_TEST(dto.hasItemTypeId());

        BOOST_TEST(dto.itemTypeId().value() == testValue);

        // Проверка clear
        dto.clearItemTypeId();
        BOOST_TEST(!dto.hasItemTypeId());
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
    // Проверка поля: valueType
    {
        BOOST_TEST(!dto.hasValueType());

        std::string testValue ="test_value";
        dto.setValueType(testValue);

        BOOST_TEST(dto.hasValueType());

        BOOST_TEST(dto.valueType().value() == testValue);

        // Проверка clear
        dto.clearValueType();
        BOOST_TEST(!dto.hasValueType());
    }
    // Проверка поля: isBoardVisible
    {
        BOOST_TEST(!dto.hasIsBoardVisible());

        bool testValue =true;
        dto.setIsBoardVisible(testValue);

        BOOST_TEST(dto.hasIsBoardVisible());

        BOOST_TEST(dto.isBoardVisible().value() == testValue);

        // Проверка clear
        dto.clearIsBoardVisible();
        BOOST_TEST(!dto.hasIsBoardVisible());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    FieldType dto;

    // Поле: id
    dto.setId(42);
    // Поле: itemTypeId
    dto.setItemTypeId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: description
    dto.setDescription("test_description");
    // Поле: valueType
    dto.setValueType("test_value_type");
    // Поле: isBoardVisible
    dto.setIsBoardVisible(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("item_type_id"));
    BOOST_TEST(json["item_type_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("description"));
    BOOST_TEST(json["description"].get<std::string>() == "test_description");
    BOOST_TEST(json.contains("value_type"));
    BOOST_TEST(json["value_type"].get<std::string>() == "test_value_type");
    BOOST_TEST(json.contains("is_board_visible"));
    BOOST_TEST(json["is_board_visible"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["item_type_id"] = 42;
    json["caption"] = "test_caption";
    json["description"] = "test_description";
    json["value_type"] = "test_value_type";
    json["is_board_visible"] = true;

    FieldType dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasItemTypeId());
    BOOST_TEST(dto.itemTypeId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasDescription());
    BOOST_TEST(dto.description().value() == "test_description");
    BOOST_TEST(dto.hasValueType());
    BOOST_TEST(dto.valueType().value() == "test_value_type");
    BOOST_TEST(dto.hasIsBoardVisible());
    BOOST_TEST(dto.isBoardVisible().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    FieldType original;

    // Поле: id
    original.setId(42);
    // Поле: itemTypeId
    original.setItemTypeId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: description
    original.setDescription("test_description");
    // Поле: valueType
    original.setValueType("test_value_type");
    // Поле: isBoardVisible
    original.setIsBoardVisible(true);

    nlohmann::json json = original.toJson();
    FieldType deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    FieldType dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setItemTypeId(42);
    dto.setCaption("test_caption");
    dto.setValueType("test_value_type");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    FieldType dto1;
    FieldType dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле itemTypeId, чтобы сделать их разными
    dto1.setItemTypeId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    FieldType dto;

    dto.setItemTypeId(42);
    dto.setCaption("test_value");
    dto.setValueType("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()