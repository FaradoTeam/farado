#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/link_type.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(LinkTypeTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    LinkType dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasSourceItemTypeId());
    BOOST_TEST(!dto.hasDestinationItemTypeId());
    BOOST_TEST(!dto.hasIsBidirectional());
    BOOST_TEST(!dto.hasCaption());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    LinkType dto;

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
    // Проверка поля: sourceItemTypeId
    {
        BOOST_TEST(!dto.hasSourceItemTypeId());

        int64_t testValue =42;
        dto.setSourceItemTypeId(testValue);

        BOOST_TEST(dto.hasSourceItemTypeId());

        BOOST_TEST(dto.sourceItemTypeId().value() == testValue);

        // Проверка clear
        dto.clearSourceItemTypeId();
        BOOST_TEST(!dto.hasSourceItemTypeId());
    }
    // Проверка поля: destinationItemTypeId
    {
        BOOST_TEST(!dto.hasDestinationItemTypeId());

        int64_t testValue =42;
        dto.setDestinationItemTypeId(testValue);

        BOOST_TEST(dto.hasDestinationItemTypeId());

        BOOST_TEST(dto.destinationItemTypeId().value() == testValue);

        // Проверка clear
        dto.clearDestinationItemTypeId();
        BOOST_TEST(!dto.hasDestinationItemTypeId());
    }
    // Проверка поля: isBidirectional
    {
        BOOST_TEST(!dto.hasIsBidirectional());

        bool testValue =true;
        dto.setIsBidirectional(testValue);

        BOOST_TEST(dto.hasIsBidirectional());

        BOOST_TEST(dto.isBidirectional().value() == testValue);

        // Проверка clear
        dto.clearIsBidirectional();
        BOOST_TEST(!dto.hasIsBidirectional());
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
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    LinkType dto;

    // Поле: id
    dto.setId(42);
    // Поле: sourceItemTypeId
    dto.setSourceItemTypeId(42);
    // Поле: destinationItemTypeId
    dto.setDestinationItemTypeId(42);
    // Поле: isBidirectional
    dto.setIsBidirectional(true);
    // Поле: caption
    dto.setCaption("test_caption");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("source_item_type_id"));
    BOOST_TEST(json["source_item_type_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("destination_item_type_id"));
    BOOST_TEST(json["destination_item_type_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("is_bidirectional"));
    BOOST_TEST(json["is_bidirectional"].get<bool>() == true);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["source_item_type_id"] = 42;
    json["destination_item_type_id"] = 42;
    json["is_bidirectional"] = true;
    json["caption"] = "test_caption";

    LinkType dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasSourceItemTypeId());
    BOOST_TEST(dto.sourceItemTypeId().value() == 42);
    BOOST_TEST(dto.hasDestinationItemTypeId());
    BOOST_TEST(dto.destinationItemTypeId().value() == 42);
    BOOST_TEST(dto.hasIsBidirectional());
    BOOST_TEST(dto.isBidirectional().value() == true);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    LinkType original;

    // Поле: id
    original.setId(42);
    // Поле: sourceItemTypeId
    original.setSourceItemTypeId(42);
    // Поле: destinationItemTypeId
    original.setDestinationItemTypeId(42);
    // Поле: isBidirectional
    original.setIsBidirectional(true);
    // Поле: caption
    original.setCaption("test_caption");

    nlohmann::json json = original.toJson();
    LinkType deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    LinkType dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setSourceItemTypeId(42);
    dto.setDestinationItemTypeId(42);
    dto.setIsBidirectional(true);
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    LinkType dto1;
    LinkType dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле sourceItemTypeId, чтобы сделать их разными
    dto1.setSourceItemTypeId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    LinkType dto;

    dto.setSourceItemTypeId(42);
    dto.setDestinationItemTypeId(42);
    dto.setIsBidirectional(true);
    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()