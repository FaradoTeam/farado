#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/auth_request.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(AuthRequestTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    AuthRequest dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasLogin());
    BOOST_TEST(!dto.hasPassword());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    AuthRequest dto;

    // Проверка поля: login
    {
        BOOST_TEST(!dto.hasLogin());

        std::string testValue ="test_value";
        dto.setLogin(testValue);

        BOOST_TEST(dto.hasLogin());

        BOOST_TEST(dto.login().value() == testValue);

        // Проверка clear
        dto.clearLogin();
        BOOST_TEST(!dto.hasLogin());
    }
    // Проверка поля: password
    {
        BOOST_TEST(!dto.hasPassword());

        std::string testValue ="test_value";
        dto.setPassword(testValue);

        BOOST_TEST(dto.hasPassword());

        BOOST_TEST(dto.password().value() == testValue);

        // Проверка clear
        dto.clearPassword();
        BOOST_TEST(!dto.hasPassword());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    AuthRequest dto;

    // Поле: login
    dto.setLogin("test_login");
    // Поле: password
    dto.setPassword("test_password");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("login"));
    BOOST_TEST(json["login"].get<std::string>() == "test_login");
    BOOST_TEST(json.contains("password"));
    BOOST_TEST(json["password"].get<std::string>() == "test_password");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["login"] = "test_login";
    json["password"] = "test_password";

    AuthRequest dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasLogin());
    BOOST_TEST(dto.login().value() == "test_login");
    BOOST_TEST(dto.hasPassword());
    BOOST_TEST(dto.password().value() == "test_password");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    AuthRequest original;

    // Поле: login
    original.setLogin("test_login");
    // Поле: password
    original.setPassword("test_password");

    nlohmann::json json = original.toJson();
    AuthRequest deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    AuthRequest dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setLogin("test_login");
    dto.setPassword("test_password");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    AuthRequest dto1;
    AuthRequest dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле login, чтобы сделать их разными
    dto1.setLogin("different_value");

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    AuthRequest dto;

    dto.setLogin("test_value");
    dto.setPassword("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()