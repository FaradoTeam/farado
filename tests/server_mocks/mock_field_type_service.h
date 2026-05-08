#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/dto/field_type.h"

#include "logic/ifield_type_service.h"

namespace server
{
namespace tests
{

class MockFieldTypeService : public services::IFieldTypeService
{
public:
    using FieldTypesPage = services::FieldTypesPage;

    void setGetFieldTypesResult(const FieldTypesPage& result)
    {
        m_getFieldTypesResult = result;
    }

    void setGetFieldTypeResult(std::optional<dto::FieldType> fieldType)
    {
        m_getFieldTypeResult = std::move(fieldType);
    }

    void setCreateFieldTypeResult(std::optional<dto::FieldType> fieldType)
    {
        m_createFieldTypeResult = std::move(fieldType);
    }

    void setUpdateFieldTypeResult(std::optional<dto::FieldType> fieldType)
    {
        m_updateFieldTypeResult = std::move(fieldType);
    }

    void setDeleteFieldTypeResult(bool result)
    {
        m_deleteFieldTypeResult = result;
    }

    void setFieldTypesByItemTypeResult(std::vector<dto::FieldType> fieldTypes)
    {
        m_fieldTypesByItemTypeResult = std::move(fieldTypes);
    }

    // Реализация интерфейса
    FieldTypesPage fieldTypes(
        int page,
        int pageSize,
        std::optional<int64_t> itemTypeId,
        std::optional<std::string> valueType,
        const std::string& searchCaption
    ) override
    {
        m_lastGetFieldTypesPage = page;
        m_lastGetFieldTypesPageSize = pageSize;
        m_lastGetFieldTypesItemTypeId = itemTypeId;
        m_lastGetFieldTypesValueType = valueType;
        m_lastGetFieldTypesSearch = searchCaption;
        m_getFieldTypesCallCount++;
        return m_getFieldTypesResult;
    }

    std::optional<dto::FieldType> fieldType(int64_t id) override
    {
        m_lastGetFieldTypeId = id;
        m_getFieldTypeCallCount++;
        return m_getFieldTypeResult;
    }

    std::optional<dto::FieldType> createFieldType(const dto::FieldType& fieldType) override
    {
        m_lastCreatedFieldType = fieldType;
        m_createFieldTypeCallCount++;
        return m_createFieldTypeResult;
    }

    std::optional<dto::FieldType> updateFieldType(const dto::FieldType& fieldType) override
    {
        m_lastUpdatedFieldType = fieldType;
        m_updateFieldTypeCallCount++;
        return m_updateFieldTypeResult;
    }

    bool deleteFieldType(int64_t id) override
    {
        m_lastDeletedFieldTypeId = id;
        m_deleteFieldTypeCallCount++;
        return m_deleteFieldTypeResult;
    }

    std::vector<dto::FieldType> fieldTypesByItemType(int64_t itemTypeId) override
    {
        m_lastFieldTypesByItemTypeId = itemTypeId;
        m_fieldTypesByItemTypeCallCount++;
        return m_fieldTypesByItemTypeResult;
    }

    // Методы для проверки вызовов
    int getFieldTypesCallCount() const { return m_getFieldTypesCallCount; }
    int getFieldTypeCallCount() const { return m_getFieldTypeCallCount; }
    int createFieldTypeCallCount() const { return m_createFieldTypeCallCount; }
    int updateFieldTypeCallCount() const { return m_updateFieldTypeCallCount; }
    int deleteFieldTypeCallCount() const { return m_deleteFieldTypeCallCount; }

    int getLastGetFieldTypesPage() const { return m_lastGetFieldTypesPage; }
    int64_t getLastGetFieldTypeId() const { return m_lastGetFieldTypeId; }
    const dto::FieldType& getLastCreatedFieldType() const { return m_lastCreatedFieldType; }
    const dto::FieldType& getLastUpdatedFieldType() const { return m_lastUpdatedFieldType; }
    int64_t getLastDeletedFieldTypeId() const { return m_lastDeletedFieldTypeId; }

    void reset()
    {
        m_getFieldTypesCallCount = 0;
        m_getFieldTypeCallCount = 0;
        m_createFieldTypeCallCount = 0;
        m_updateFieldTypeCallCount = 0;
        m_deleteFieldTypeCallCount = 0;
        m_fieldTypesByItemTypeCallCount = 0;

        m_lastGetFieldTypesPage = 0;
        m_lastGetFieldTypesPageSize = 0;
        m_lastGetFieldTypeId = 0;
        m_lastDeletedFieldTypeId = 0;
    }

private:
    FieldTypesPage m_getFieldTypesResult;
    std::optional<dto::FieldType> m_getFieldTypeResult;
    std::optional<dto::FieldType> m_createFieldTypeResult;
    std::optional<dto::FieldType> m_updateFieldTypeResult;
    bool m_deleteFieldTypeResult = false;
    std::vector<dto::FieldType> m_fieldTypesByItemTypeResult;

    int m_getFieldTypesCallCount = 0;
    int m_getFieldTypeCallCount = 0;
    int m_createFieldTypeCallCount = 0;
    int m_updateFieldTypeCallCount = 0;
    int m_deleteFieldTypeCallCount = 0;
    int m_fieldTypesByItemTypeCallCount = 0;

    int m_lastGetFieldTypesPage = 0;
    int m_lastGetFieldTypesPageSize = 0;
    std::optional<int64_t> m_lastGetFieldTypesItemTypeId;
    std::optional<std::string> m_lastGetFieldTypesValueType;
    std::string m_lastGetFieldTypesSearch;
    int64_t m_lastGetFieldTypeId = 0;
    dto::FieldType m_lastCreatedFieldType;
    dto::FieldType m_lastUpdatedFieldType;
    int64_t m_lastDeletedFieldTypeId = 0;
    int64_t m_lastFieldTypesByItemTypeId = 0;
};

} // namespace tests
} // namespace server
