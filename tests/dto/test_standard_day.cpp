#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/standard_day.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(StandardDayTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    StandardDay dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasWeekDayNumber());
    BOOST_TEST(!dto.hasWeekOrder());
    BOOST_TEST(!dto.hasIsWorkDay());
    BOOST_TEST(!dto.hasBeginWorkTime());
    BOOST_TEST(!dto.hasEndWorkTime());
    BOOST_TEST(!dto.hasBreakDuration());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    StandardDay dto;

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
    // Проверка поля: weekDayNumber
    {
        BOOST_TEST(!dto.hasWeekDayNumber());

        int64_t testValue =42;
        dto.setWeekDayNumber(testValue);

        BOOST_TEST(dto.hasWeekDayNumber());

        BOOST_TEST(dto.weekDayNumber().value() == testValue);

        // Проверка clear
        dto.clearWeekDayNumber();
        BOOST_TEST(!dto.hasWeekDayNumber());
    }
    // Проверка поля: weekOrder
    {
        BOOST_TEST(!dto.hasWeekOrder());

        int64_t testValue =42;
        dto.setWeekOrder(testValue);

        BOOST_TEST(dto.hasWeekOrder());

        BOOST_TEST(dto.weekOrder().value() == testValue);

        // Проверка clear
        dto.clearWeekOrder();
        BOOST_TEST(!dto.hasWeekOrder());
    }
    // Проверка поля: isWorkDay
    {
        BOOST_TEST(!dto.hasIsWorkDay());

        bool testValue =true;
        dto.setIsWorkDay(testValue);

        BOOST_TEST(dto.hasIsWorkDay());

        BOOST_TEST(dto.isWorkDay().value() == testValue);

        // Проверка clear
        dto.clearIsWorkDay();
        BOOST_TEST(!dto.hasIsWorkDay());
    }
    // Проверка поля: beginWorkTime
    {
        BOOST_TEST(!dto.hasBeginWorkTime());

        std::string testValue ="test_value";
        dto.setBeginWorkTime(testValue);

        BOOST_TEST(dto.hasBeginWorkTime());

        BOOST_TEST(dto.beginWorkTime().value() == testValue);

        // Проверка clear
        dto.clearBeginWorkTime();
        BOOST_TEST(!dto.hasBeginWorkTime());
    }
    // Проверка поля: endWorkTime
    {
        BOOST_TEST(!dto.hasEndWorkTime());

        std::string testValue ="test_value";
        dto.setEndWorkTime(testValue);

        BOOST_TEST(dto.hasEndWorkTime());

        BOOST_TEST(dto.endWorkTime().value() == testValue);

        // Проверка clear
        dto.clearEndWorkTime();
        BOOST_TEST(!dto.hasEndWorkTime());
    }
    // Проверка поля: breakDuration
    {
        BOOST_TEST(!dto.hasBreakDuration());

        int64_t testValue =42;
        dto.setBreakDuration(testValue);

        BOOST_TEST(dto.hasBreakDuration());

        BOOST_TEST(dto.breakDuration().value() == testValue);

        // Проверка clear
        dto.clearBreakDuration();
        BOOST_TEST(!dto.hasBreakDuration());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    StandardDay dto;

    // Поле: id
    dto.setId(42);
    // Поле: weekDayNumber
    dto.setWeekDayNumber(42);
    // Поле: weekOrder
    dto.setWeekOrder(42);
    // Поле: isWorkDay
    dto.setIsWorkDay(true);
    // Поле: beginWorkTime
    dto.setBeginWorkTime("test_begin_work_time");
    // Поле: endWorkTime
    dto.setEndWorkTime("test_end_work_time");
    // Поле: breakDuration
    dto.setBreakDuration(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("week_day_number"));
    BOOST_TEST(json["week_day_number"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("week_order"));
    BOOST_TEST(json["week_order"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("is_work_day"));
    BOOST_TEST(json["is_work_day"].get<bool>() == true);
    BOOST_TEST(json.contains("begin_work_time"));
    BOOST_TEST(json["begin_work_time"].get<std::string>() == "test_begin_work_time");
    BOOST_TEST(json.contains("end_work_time"));
    BOOST_TEST(json["end_work_time"].get<std::string>() == "test_end_work_time");
    BOOST_TEST(json.contains("break_duration"));
    BOOST_TEST(json["break_duration"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["week_day_number"] = 42;
    json["week_order"] = 42;
    json["is_work_day"] = true;
    json["begin_work_time"] = "test_begin_work_time";
    json["end_work_time"] = "test_end_work_time";
    json["break_duration"] = 42;

    StandardDay dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasWeekDayNumber());
    BOOST_TEST(dto.weekDayNumber().value() == 42);
    BOOST_TEST(dto.hasWeekOrder());
    BOOST_TEST(dto.weekOrder().value() == 42);
    BOOST_TEST(dto.hasIsWorkDay());
    BOOST_TEST(dto.isWorkDay().value() == true);
    BOOST_TEST(dto.hasBeginWorkTime());
    BOOST_TEST(dto.beginWorkTime().value() == "test_begin_work_time");
    BOOST_TEST(dto.hasEndWorkTime());
    BOOST_TEST(dto.endWorkTime().value() == "test_end_work_time");
    BOOST_TEST(dto.hasBreakDuration());
    BOOST_TEST(dto.breakDuration().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    StandardDay original;

    // Поле: id
    original.setId(42);
    // Поле: weekDayNumber
    original.setWeekDayNumber(42);
    // Поле: weekOrder
    original.setWeekOrder(42);
    // Поле: isWorkDay
    original.setIsWorkDay(true);
    // Поле: beginWorkTime
    original.setBeginWorkTime("test_begin_work_time");
    // Поле: endWorkTime
    original.setEndWorkTime("test_end_work_time");
    // Поле: breakDuration
    original.setBreakDuration(42);

    nlohmann::json json = original.toJson();
    StandardDay deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    StandardDay dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setWeekDayNumber(42);
    dto.setWeekOrder(42);
    dto.setIsWorkDay(true);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    StandardDay dto1;
    StandardDay dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле weekDayNumber, чтобы сделать их разными
    dto1.setWeekDayNumber(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    StandardDay dto;

    dto.setWeekDayNumber(42);
    dto.setWeekOrder(42);
    dto.setIsWorkDay(true);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()