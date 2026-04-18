#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/board_column.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(BoardColumnTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    BoardColumn dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasBoardId());
    BOOST_TEST(!dto.hasStateId());
    BOOST_TEST(!dto.hasOrderNumber());
    BOOST_TEST(!dto.hasSettings());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    BoardColumn dto;

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
    // Проверка поля: boardId
    {
        BOOST_TEST(!dto.hasBoardId());

        int64_t testValue =42;
        dto.setBoardId(testValue);

        BOOST_TEST(dto.hasBoardId());

        BOOST_TEST(dto.boardId().value() == testValue);

        // Проверка clear
        dto.clearBoardId();
        BOOST_TEST(!dto.hasBoardId());
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
    // Проверка поля: orderNumber
    {
        BOOST_TEST(!dto.hasOrderNumber());

        int64_t testValue =42;
        dto.setOrderNumber(testValue);

        BOOST_TEST(dto.hasOrderNumber());

        BOOST_TEST(dto.orderNumber().value() == testValue);

        // Проверка clear
        dto.clearOrderNumber();
        BOOST_TEST(!dto.hasOrderNumber());
    }
    // Проверка поля: settings
    {
        BOOST_TEST(!dto.hasSettings());

        std::string testValue ="test_value";
        dto.setSettings(testValue);

        BOOST_TEST(dto.hasSettings());

        BOOST_TEST(dto.settings().value() == testValue);

        // Проверка clear
        dto.clearSettings();
        BOOST_TEST(!dto.hasSettings());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    BoardColumn dto;

    // Поле: id
    dto.setId(42);
    // Поле: boardId
    dto.setBoardId(42);
    // Поле: stateId
    dto.setStateId(42);
    // Поле: orderNumber
    dto.setOrderNumber(42);
    // Поле: settings
    dto.setSettings("test_settings");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("board_id"));
    BOOST_TEST(json["board_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("state_id"));
    BOOST_TEST(json["state_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("order_number"));
    BOOST_TEST(json["order_number"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("settings"));
    BOOST_TEST(json["settings"].get<std::string>() == "test_settings");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["board_id"] = 42;
    json["state_id"] = 42;
    json["order_number"] = 42;
    json["settings"] = "test_settings";

    BoardColumn dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasBoardId());
    BOOST_TEST(dto.boardId().value() == 42);
    BOOST_TEST(dto.hasStateId());
    BOOST_TEST(dto.stateId().value() == 42);
    BOOST_TEST(dto.hasOrderNumber());
    BOOST_TEST(dto.orderNumber().value() == 42);
    BOOST_TEST(dto.hasSettings());
    BOOST_TEST(dto.settings().value() == "test_settings");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    BoardColumn original;

    // Поле: id
    original.setId(42);
    // Поле: boardId
    original.setBoardId(42);
    // Поле: stateId
    original.setStateId(42);
    // Поле: orderNumber
    original.setOrderNumber(42);
    // Поле: settings
    original.setSettings("test_settings");

    nlohmann::json json = original.toJson();
    BoardColumn deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    BoardColumn dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setBoardId(42);
    dto.setStateId(42);
    dto.setOrderNumber(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    BoardColumn dto1;
    BoardColumn dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле boardId, чтобы сделать их разными
    dto1.setBoardId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    BoardColumn dto;

    dto.setBoardId(42);
    dto.setStateId(42);
    dto.setOrderNumber(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()