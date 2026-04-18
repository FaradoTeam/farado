#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>

#include "common/dto/page_response.h"

#include <optional>
#include "common/helpers/time_helpers.h"

using namespace dto;

BOOST_AUTO_TEST_SUITE(PageResponseTests)

// Тест: Конструктор по умолчанию
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    PageResponse dto;

    // Все optional поля должны быть пустыми
    BOOST_TEST(!dto.hasItems());
    BOOST_TEST(!dto.hasTotalCount());
    BOOST_TEST(!dto.hasPage());
    BOOST_TEST(!dto.hasPageSize());
}

// Тест: Геттеры и сеттеры
BOOST_AUTO_TEST_CASE(GettersAndSetters)
{
    PageResponse dto;

    // Проверка поля: items
    {
        BOOST_TEST(!dto.hasItems());

        std::vector<std::string> testValue ={"item1", "item2"};
        dto.setItems(testValue);

        BOOST_TEST(dto.hasItems());

        BOOST_TEST(dto.items().value() == testValue);

        // Проверка clear
        dto.clearItems();
        BOOST_TEST(!dto.hasItems());
    }
    // Проверка поля: totalCount
    {
        BOOST_TEST(!dto.hasTotalCount());

        int64_t testValue =42;
        dto.setTotalCount(testValue);

        BOOST_TEST(dto.hasTotalCount());

        BOOST_TEST(dto.totalCount().value() == testValue);

        // Проверка clear
        dto.clearTotalCount();
        BOOST_TEST(!dto.hasTotalCount());
    }
    // Проверка поля: page
    {
        BOOST_TEST(!dto.hasPage());

        int64_t testValue =42;
        dto.setPage(testValue);

        BOOST_TEST(dto.hasPage());

        BOOST_TEST(dto.page().value() == testValue);

        // Проверка clear
        dto.clearPage();
        BOOST_TEST(!dto.hasPage());
    }
    // Проверка поля: pageSize
    {
        BOOST_TEST(!dto.hasPageSize());

        int64_t testValue =42;
        dto.setPageSize(testValue);

        BOOST_TEST(dto.hasPageSize());

        BOOST_TEST(dto.pageSize().value() == testValue);

        // Проверка clear
        dto.clearPageSize();
        BOOST_TEST(!dto.hasPageSize());
    }
}

// Тест: сериализация в JSON
BOOST_AUTO_TEST_CASE(ToJsonSerialization)
{
    PageResponse dto;

    // Поле: items
    dto.setItems({"item1", "item2"});
    // Поле: totalCount
    dto.setTotalCount(42);
    // Поле: page
    dto.setPage(42);
    // Поле: pageSize
    dto.setPageSize(42);

    nlohmann::json json = dto.toJson();

    // Проверка полей JSON
    BOOST_TEST(json.contains("items"));
    BOOST_TEST(json["items"].is_array());
    BOOST_TEST(json["items"].size() == 2);
    BOOST_TEST(json.contains("total_count"));
    BOOST_TEST(json["total_count"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("page"));
    BOOST_TEST(json["page"].get<int64_t>() == 42);
    BOOST_TEST(json.contains("page_size"));
    BOOST_TEST(json["page_size"].get<int64_t>() == 42);
}

// Тест: десериализация из JSON
BOOST_AUTO_TEST_CASE(FromJsonDeserialization)
{
    nlohmann::json json = nlohmann::json::object();
    json["items"] = nlohmann::json::array({"item1", "item2"});
    json["total_count"] = 42;
    json["page"] = 42;
    json["page_size"] = 42;

    PageResponse dto(json);

    // Проверка десериализованных значений
    BOOST_TEST(dto.hasItems());
    BOOST_TEST(dto.items().value().size() == 2);
    BOOST_TEST(dto.hasTotalCount());
    BOOST_TEST(dto.totalCount().value() == 42);
    BOOST_TEST(dto.hasPage());
    BOOST_TEST(dto.page().value() == 42);
    BOOST_TEST(dto.hasPageSize());
    BOOST_TEST(dto.pageSize().value() == 42);
}

// Тест: Сериализация в оба конца
BOOST_AUTO_TEST_CASE(RoundTripSerialization)
{
    PageResponse original;

    // Поле: items
    original.setItems({"item1", "item2"});
    // Поле: totalCount
    original.setTotalCount(42);
    // Поле: page
    original.setPage(42);
    // Поле: pageSize
    original.setPageSize(42);

    nlohmann::json json = original.toJson();
    PageResponse deserialized(json);

    // Сравнение исходного и десериализованного
    BOOST_TEST(original == deserialized);
}

// Тест: проверка isValid
BOOST_AUTO_TEST_CASE(Validation)
{
    PageResponse dto;

    // Проверяем, есть ли обязательные поля

    // Изначально невалиден, если есть обязательные поля
    BOOST_TEST(dto.isValid());
    BOOST_TEST(dto.validationError().empty());

    // Заполняем обязательные поля

    // Теперь должен быть валидным
    // Если обязательных полей нет, DTO всегда валиден
    BOOST_TEST(dto.isValid());
}

// Тест: Операторы сравнения
BOOST_AUTO_TEST_CASE(ComparisonOperators)
{
    PageResponse dto1;
    PageResponse dto2;

    // Изначально они должны быть равны (оба пустые)
    BOOST_TEST(dto1 == dto2);
    BOOST_TEST(!(dto1 != dto2));


    // Если нет обязательных полей, используем первое неконстантное поле
    dto1.setItems({"different"});

    BOOST_TEST(dto1 != dto2);
    BOOST_TEST(!(dto1 == dto2));
}

// Тест: Оператор потокового вывода
BOOST_AUTO_TEST_CASE(StreamOutput)
{
    PageResponse dto;


    std::stringstream ss;
    ss << dto;
    BOOST_TEST(!ss.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()