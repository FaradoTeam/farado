#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/user.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(UserTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    User dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasId());
    BOOST_TEST(!dto.hasLogin());
    BOOST_TEST(!dto.hasFirstName());
    BOOST_TEST(!dto.hasMiddleName());
    BOOST_TEST(!dto.hasLastName());
    BOOST_TEST(!dto.hasEmail());
    BOOST_TEST(!dto.hasNeedChangePassword());
    BOOST_TEST(!dto.hasIsBlocked());
    BOOST_TEST(!dto.hasIsSuperAdmin());
    BOOST_TEST(!dto.hasIsHidden());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    User dto;

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
    // Проверка поля: firstName
    {
        BOOST_TEST(!dto.hasFirstName());

        std::string testValue ="test_value";
        dto.setFirstName(testValue);

        BOOST_TEST(dto.hasFirstName());

        BOOST_TEST(dto.firstName().value() == testValue);

        // Проверка clear
        dto.clearFirstName();
        BOOST_TEST(!dto.hasFirstName());
    }
    // Проверка поля: middleName
    {
        BOOST_TEST(!dto.hasMiddleName());

        std::string testValue ="test_value";
        dto.setMiddleName(testValue);

        BOOST_TEST(dto.hasMiddleName());

        BOOST_TEST(dto.middleName().value() == testValue);

        // Проверка clear
        dto.clearMiddleName();
        BOOST_TEST(!dto.hasMiddleName());
    }
    // Проверка поля: lastName
    {
        BOOST_TEST(!dto.hasLastName());

        std::string testValue ="test_value";
        dto.setLastName(testValue);

        BOOST_TEST(dto.hasLastName());

        BOOST_TEST(dto.lastName().value() == testValue);

        // Проверка clear
        dto.clearLastName();
        BOOST_TEST(!dto.hasLastName());
    }
    // Проверка поля: email
    {
        BOOST_TEST(!dto.hasEmail());

        std::string testValue ="test_value";
        dto.setEmail(testValue);

        BOOST_TEST(dto.hasEmail());

        BOOST_TEST(dto.email().value() == testValue);

        // Проверка clear
        dto.clearEmail();
        BOOST_TEST(!dto.hasEmail());
    }
    // Проверка поля: needChangePassword
    {
        BOOST_TEST(!dto.hasNeedChangePassword());

        bool testValue =true;
        dto.setNeedChangePassword(testValue);

        BOOST_TEST(dto.hasNeedChangePassword());

        BOOST_TEST(dto.needChangePassword().value() == testValue);

        // Проверка clear
        dto.clearNeedChangePassword();
        BOOST_TEST(!dto.hasNeedChangePassword());
    }
    // Проверка поля: isBlocked
    {
        BOOST_TEST(!dto.hasIsBlocked());

        bool testValue =true;
        dto.setIsBlocked(testValue);

        BOOST_TEST(dto.hasIsBlocked());

        BOOST_TEST(dto.isBlocked().value() == testValue);

        // Проверка clear
        dto.clearIsBlocked();
        BOOST_TEST(!dto.hasIsBlocked());
    }
    // Проверка поля: isSuperAdmin
    {
        BOOST_TEST(!dto.hasIsSuperAdmin());

        bool testValue =true;
        dto.setIsSuperAdmin(testValue);

        BOOST_TEST(dto.hasIsSuperAdmin());

        BOOST_TEST(dto.isSuperAdmin().value() == testValue);

        // Проверка clear
        dto.clearIsSuperAdmin();
        BOOST_TEST(!dto.hasIsSuperAdmin());
    }
    // Проверка поля: isHidden
    {
        BOOST_TEST(!dto.hasIsHidden());

        bool testValue =true;
        dto.setIsHidden(testValue);

        BOOST_TEST(dto.hasIsHidden());

        BOOST_TEST(dto.isHidden().value() == testValue);

        // Проверка clear
        dto.clearIsHidden();
        BOOST_TEST(!dto.hasIsHidden());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    User dto;

    // Поле: id
    dto.setId(42);
    // Поле: login
    dto.setLogin("test_login");
    // Поле: firstName
    dto.setFirstName("test_first_name");
    // Поле: middleName
    dto.setMiddleName("test_middle_name");
    // Поле: lastName
    dto.setLastName("test_last_name");
    // Поле: email
    dto.setEmail("test_email");
    // Поле: needChangePassword
    dto.setNeedChangePassword(true);
    // Поле: isBlocked
    dto.setIsBlocked(true);
    // Поле: isSuperAdmin
    dto.setIsSuperAdmin(true);
    // Поле: isHidden
    dto.setIsHidden(true);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("id"));
    BOOST_TEST(json["id"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("login"));
    BOOST_TEST(json["login"].get<std::string>() == "test_login");
    BOOST_TEST(json.contains("first_name"));
    BOOST_TEST(json["first_name"].get<std::string>() == "test_first_name");
    BOOST_TEST(json.contains("middle_name"));
    BOOST_TEST(json["middle_name"].get<std::string>() == "test_middle_name");
    BOOST_TEST(json.contains("last_name"));
    BOOST_TEST(json["last_name"].get<std::string>() == "test_last_name");
    BOOST_TEST(json.contains("email"));
    BOOST_TEST(json["email"].get<std::string>() == "test_email");
    BOOST_TEST(json.contains("need_change_password"));
    BOOST_TEST(json["need_change_password"].get<bool>() == true);
    BOOST_TEST(json.contains("is_blocked"));
    BOOST_TEST(json["is_blocked"].get<bool>() == true);
    BOOST_TEST(json.contains("is_super_admin"));
    BOOST_TEST(json["is_super_admin"].get<bool>() == true);
    BOOST_TEST(json.contains("is_hidden"));
    BOOST_TEST(json["is_hidden"].get<bool>() == true);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["id"] = 42;
    json["login"] = "test_login";
    json["first_name"] = "test_first_name";
    json["middle_name"] = "test_middle_name";
    json["last_name"] = "test_last_name";
    json["email"] = "test_email";
    json["need_change_password"] = true;
    json["is_blocked"] = true;
    json["is_super_admin"] = true;
    json["is_hidden"] = true;

    User dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasId());
    BOOST_TEST(dto.id().value() == 42);
    BOOST_TEST(dto.hasLogin());
    BOOST_TEST(dto.login().value() == "test_login");
    BOOST_TEST(dto.hasFirstName());
    BOOST_TEST(dto.firstName().value() == "test_first_name");
    BOOST_TEST(dto.hasMiddleName());
    BOOST_TEST(dto.middleName().value() == "test_middle_name");
    BOOST_TEST(dto.hasLastName());
    BOOST_TEST(dto.lastName().value() == "test_last_name");
    BOOST_TEST(dto.hasEmail());
    BOOST_TEST(dto.email().value() == "test_email");
    BOOST_TEST(dto.hasNeedChangePassword());
    BOOST_TEST(dto.needChangePassword().value() == true);
    BOOST_TEST(dto.hasIsBlocked());
    BOOST_TEST(dto.isBlocked().value() == true);
    BOOST_TEST(dto.hasIsSuperAdmin());
    BOOST_TEST(dto.isSuperAdmin().value() == true);
    BOOST_TEST(dto.hasIsHidden());
    BOOST_TEST(dto.isHidden().value() == true);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    User original;

    // Поле: id
    original.setId(42);
    // Поле: login
    original.setLogin("test_login");
    // Поле: firstName
    original.setFirstName("test_first_name");
    // Поле: middleName
    original.setMiddleName("test_middle_name");
    // Поле: lastName
    original.setLastName("test_last_name");
    // Поле: email
    original.setEmail("test_email");
    // Поле: needChangePassword
    original.setNeedChangePassword(true);
    // Поле: isBlocked
    original.setIsBlocked(true);
    // Поле: isSuperAdmin
    original.setIsSuperAdmin(true);
    // Поле: isHidden
    original.setIsHidden(true);

    nlohmann::json json = original.toJson();
    User deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    User dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(!dto.isValid());
    BOOST_TEST(dto.validationError().find("обязательным") != std::string::npos);

    // Заполняем обязательные поля
    dto.setLogin("test_login");

    // Теперь должен быть валидным
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    User dto1;
    User dto2;

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
    User dto;

    dto.setLogin("test_value");

    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()