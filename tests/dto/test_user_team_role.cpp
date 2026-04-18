#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/user_team_role.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(UserTeamRoleTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    UserTeamRole dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasUserId());
    BOOST_TEST(!dto.hasTeamId());
    BOOST_TEST(!dto.hasRoleId());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    UserTeamRole dto;

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
    // Проверка поля: userId
    {
        BOOST_TEST(!dto.hasUserId());

        int64_t testValue =42;
        dto.setUserId(testValue);

        BOOST_TEST(dto.hasUserId());

        BOOST_TEST(dto.userId().value() == testValue);

        // Проверка clear
        dto.clearUserId();
        BOOST_TEST(!dto.hasUserId());
    }
    // Проверка поля: teamId
    {
        BOOST_TEST(!dto.hasTeamId());

        int64_t testValue =42;
        dto.setTeamId(testValue);

        BOOST_TEST(dto.hasTeamId());

        BOOST_TEST(dto.teamId().value() == testValue);

        // Проверка clear
        dto.clearTeamId();
        BOOST_TEST(!dto.hasTeamId());
    }
    // Проверка поля: roleId
    {
        BOOST_TEST(!dto.hasRoleId());

        int64_t testValue =42;
        dto.setRoleId(testValue);

        BOOST_TEST(dto.hasRoleId());

        BOOST_TEST(dto.roleId().value() == testValue);

        // Проверка clear
        dto.clearRoleId();
        BOOST_TEST(!dto.hasRoleId());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    UserTeamRole dto;

    // Поле: id
    dto.setId(42);
    // Поле: userId
    dto.setUserId(42);
    // Поле: teamId
    dto.setTeamId(42);
    // Поле: roleId
    dto.setRoleId(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("user_id"));
    BOOST_TEST(json["user_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("team_id"));
    BOOST_TEST(json["team_id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("role_id"));
    BOOST_TEST(json["role_id"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["user_id"] = 42;
    json["team_id"] = 42;
    json["role_id"] = 42;

    UserTeamRole dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasUserId());
    BOOST_TEST(dto.userId().value() == 42);
    BOOST_TEST(dto.hasTeamId());
    BOOST_TEST(dto.teamId().value() == 42);
    BOOST_TEST(dto.hasRoleId());
    BOOST_TEST(dto.roleId().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    UserTeamRole original;

    // Поле: id
    original.setId(42);
    // Поле: userId
    original.setUserId(42);
    // Поле: teamId
    original.setTeamId(42);
    // Поле: roleId
    original.setRoleId(42);

    nlohmann::json json = original.toJson();
    UserTeamRole deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    UserTeamRole dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setUserId(42);
    dto.setTeamId(42);
    dto.setRoleId(42);

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    UserTeamRole dto1;
    UserTeamRole dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле userId, чтобы сделать их разными
    dto1.setUserId(999);

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    UserTeamRole dto;

    dto.setUserId(42);
    dto.setTeamId(42);
    dto.setRoleId(42);

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()