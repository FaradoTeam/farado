#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/change_password_request.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(ChangePasswordRequestTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    ChangePasswordRequest dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasOldPassword());
    BOOST_TEST(!dto.hasNewPassword());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    ChangePasswordRequest dto;

    // Проверка поля: oldPassword
    {
        BOOST_TEST(!dto.hasOldPassword());

        std::string testValue ="test_value";
        dto.setOldPassword(testValue);

        BOOST_TEST(dto.hasOldPassword());

        BOOST_TEST(dto.oldPassword().value() == testValue);

        // Проверка clear
        dto.clearOldPassword();
        BOOST_TEST(!dto.hasOldPassword());
    }
    // Проверка поля: newPassword
    {
        BOOST_TEST(!dto.hasNewPassword());

        std::string testValue ="test_value";
        dto.setNewPassword(testValue);

        BOOST_TEST(dto.hasNewPassword());

        BOOST_TEST(dto.newPassword().value() == testValue);

        // Проверка clear
        dto.clearNewPassword();
        BOOST_TEST(!dto.hasNewPassword());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    ChangePasswordRequest dto;

    // Поле: oldPassword
    dto.setOldPassword("test_old_password");
    // Поле: newPassword
    dto.setNewPassword("test_new_password");

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("old_password"));
    BOOST_TEST(json["old_password"].get<std::string>() == "test_old_password");
    BOOST_TEST(json.contains("new_password"));
    BOOST_TEST(json["new_password"].get<std::string>() == "test_new_password");
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["old_password"] = "test_old_password";
    json["new_password"] = "test_new_password";

    ChangePasswordRequest dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasOldPassword());
    BOOST_TEST(dto.oldPassword().value() == "test_old_password");
    BOOST_TEST(dto.hasNewPassword());
    BOOST_TEST(dto.newPassword().value() == "test_new_password");
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    ChangePasswordRequest original;

    // Поле: oldPassword
    original.setOldPassword("test_old_password");
    // Поле: newPassword
    original.setNewPassword("test_new_password");

    nlohmann::json json = original.toJson();
    ChangePasswordRequest deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    ChangePasswordRequest dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setOldPassword("test_old_password");
    dto.setNewPassword("test_new_password");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    ChangePasswordRequest dto1;
    ChangePasswordRequest dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));

    // Изменим поле oldPassword, чтобы сделать их разными
    dto1.setOldPassword("different_value");

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));

}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    ChangePasswordRequest dto;

    dto.setOldPassword("test_value");
    dto.setNewPassword("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()