#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/item_link.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ItemLinkTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    ItemLink dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasLinkTypeId());
    BOOST_TEST(!dto.hasSourceItemId());
    BOOST_TEST(!dto.hasDestinationItemId());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    ItemLink dto;

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
    // Проверка поля: linkTypeId
    {
        BOOST_TEST(!dto.hasLinkTypeId());

        int64_t testValue =42;
        dto.setLinkTypeId(testValue);

        BOOST_TEST(dto.hasLinkTypeId());

        BOOST_TEST(dto.linkTypeId().value() == testValue);

        // Проверка clear
        dto.clearLinkTypeId();
        BOOST_TEST(!dto.hasLinkTypeId());
    }
    // Проверка поля: sourceItemId
    {
        BOOST_TEST(!dto.hasSourceItemId());

        int64_t testValue =42;
        dto.setSourceItemId(testValue);

        BOOST_TEST(dto.hasSourceItemId());

        BOOST_TEST(dto.sourceItemId().value() == testValue);

        // Проверка clear
        dto.clearSourceItemId();
        BOOST_TEST(!dto.hasSourceItemId());
    }
    // Проверка поля: destinationItemId
    {
        BOOST_TEST(!dto.hasDestinationItemId());

        int64_t testValue =42;
        dto.setDestinationItemId(testValue);

        BOOST_TEST(dto.hasDestinationItemId());

        BOOST_TEST(dto.destinationItemId().value() == testValue);

        // Проверка clear
        dto.clearDestinationItemId();
        BOOST_TEST(!dto.hasDestinationItemId());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    ItemLink dto;

    // Поле: id
    dto.setId(42);
    // Поле: linkTypeId
    dto.setLinkTypeId(42);
    // Поле: sourceItemId
    dto.setSourceItemId(42);
    // Поле: destinationItemId
    dto.setDestinationItemId(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("link_type_id"));
    BOOST_TEST(json["link_type_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("source_item_id"));
    BOOST_TEST(json["source_item_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("destination_item_id"));
    BOOST_TEST(json["destination_item_id"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["link_type_id"] = 42;
    json["source_item_id"] = 42;
    json["destination_item_id"] = 42;

    ItemLink dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasLinkTypeId());
    BOOST_TEST(dto.linkTypeId().value() == 42);
    BOOST_TEST(dto.hasSourceItemId());
    BOOST_TEST(dto.sourceItemId().value() == 42);
    BOOST_TEST(dto.hasDestinationItemId());
    BOOST_TEST(dto.destinationItemId().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    ItemLink original;

    // Поле: id
    original.setId(42);
    // Поле: linkTypeId
    original.setLinkTypeId(42);
    // Поле: sourceItemId
    original.setSourceItemId(42);
    // Поле: destinationItemId
    original.setDestinationItemId(42);

    nlohmann::json json = original.toJson();
    ItemLink deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    ItemLink dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setLinkTypeId(42);
    dto.setSourceItemId(42);
    dto.setDestinationItemId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    ItemLink dto1;
    ItemLink dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле linkTypeId, чтобы сделать их разными
    dto1.setLinkTypeId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    ItemLink dto;

    dto.setLinkTypeId(42);
    dto.setSourceItemId(42);
    dto.setDestinationItemId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()