#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/item.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ItemTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    Item dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasItemTypeId());
    BOOST_TEST(!dto.hasParentId());
    BOOST_TEST(!dto.hasStateId());
    BOOST_TEST(!dto.hasPhaseId());
    BOOST_TEST(!dto.hasCaption());
    BOOST_TEST(!dto.hasContent());
    BOOST_TEST(!dto.hasIsDeleted());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    Item dto;

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
    // Проверка поля: phaseId
    {
        BOOST_TEST(!dto.hasPhaseId());

        int64_t testValue =42;
        dto.setPhaseId(testValue);

        BOOST_TEST(dto.hasPhaseId());

        BOOST_TEST(dto.phaseId().value() == testValue);

        // Проверка clear
        dto.clearPhaseId();
        BOOST_TEST(!dto.hasPhaseId());
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
    // Проверка поля: content
    {
        BOOST_TEST(!dto.hasContent());

        std::string testValue ="test_value";
        dto.setContent(testValue);

        BOOST_TEST(dto.hasContent());

        BOOST_TEST(dto.content().value() == testValue);

        // Проверка clear
        dto.clearContent();
        BOOST_TEST(!dto.hasContent());
    }
    // Проверка поля: isDeleted
    {
        BOOST_TEST(!dto.hasIsDeleted());

        bool testValue =true;
        dto.setIsDeleted(testValue);

        BOOST_TEST(dto.hasIsDeleted());

        BOOST_TEST(dto.isDeleted().value() == testValue);

        // Проверка clear
        dto.clearIsDeleted();
        BOOST_TEST(!dto.hasIsDeleted());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    Item dto;

    // Поле: id
    dto.setId(42);
    // Поле: itemTypeId
    dto.setItemTypeId(42);
    // Поле: parentId
    dto.setParentId(42);
    // Поле: stateId
    dto.setStateId(42);
    // Поле: phaseId
    dto.setPhaseId(42);
    // Поле: caption
    dto.setCaption("test_caption");
    // Поле: content
    dto.setContent("test_content");
    // Поле: isDeleted
    dto.setIsDeleted(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("item_type_id"));
    BOOST_TEST(json["item_type_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("parent_id"));
    BOOST_TEST(json["parent_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("state_id"));
    BOOST_TEST(json["state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("phase_id"));
    BOOST_TEST(json["phase_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("caption"));
    BOOST_TEST(json["caption"].get<std::string>() == "test_caption");
    BOOST_TEST(json.contains("content"));
    BOOST_TEST(json["content"].get<std::string>() == "test_content");
    BOOST_TEST(json.contains("is_deleted"));
    BOOST_TEST(json["is_deleted"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["item_type_id"] = 42;
    json["parent_id"] = 42;
    json["state_id"] = 42;
    json["phase_id"] = 42;
    json["caption"] = "test_caption";
    json["content"] = "test_content";
    json["is_deleted"] = true;

    Item dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasItemTypeId());
    BOOST_TEST(dto.itemTypeId().value() == 42);
    BOOST_TEST(dto.hasParentId());
    BOOST_TEST(dto.parentId().value() == 42);
    BOOST_TEST(dto.hasStateId());
    BOOST_TEST(dto.stateId().value() == 42);
    BOOST_TEST(dto.hasPhaseId());
    BOOST_TEST(dto.phaseId().value() == 42);
    BOOST_TEST(dto.hasCaption());
    BOOST_TEST(dto.caption().value() == "test_caption");
    BOOST_TEST(dto.hasContent());
    BOOST_TEST(dto.content().value() == "test_content");
    BOOST_TEST(dto.hasIsDeleted());
    BOOST_TEST(dto.isDeleted().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    Item original;

    // Поле: id
    original.setId(42);
    // Поле: itemTypeId
    original.setItemTypeId(42);
    // Поле: parentId
    original.setParentId(42);
    // Поле: stateId
    original.setStateId(42);
    // Поле: phaseId
    original.setPhaseId(42);
    // Поле: caption
    original.setCaption("test_caption");
    // Поле: content
    original.setContent("test_content");
    // Поле: isDeleted
    original.setIsDeleted(true);

    nlohmann::json json = original.toJson();
    Item deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    Item dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setItemTypeId(42);
    dto.setStateId(42);
    dto.setPhaseId(42);
    dto.setCaption("test_caption");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    Item dto1;
    Item dto2;

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
    Item dto;

    dto.setItemTypeId(42);
    dto.setStateId(42);
    dto.setPhaseId(42);
    dto.setCaption("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()