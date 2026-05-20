#pragma once

#include <memory>

#include "logic/ifield_type_service.h"
#include "repo/field_type_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса для управления типами полей.
 */
class FieldTypeService final : public IFieldTypeService
{
public:
    explicit FieldTypeService(
        std::shared_ptr<repositories::IFieldTypeRepository> fieldTypeRepo
    );

    FieldTypesPage fieldTypes(
        int page,
        int pageSize,
        std::optional<int64_t> itemTypeId,
        std::optional<std::string> valueType,
        const std::string& searchCaption
    ) override;

    std::optional<dto::FieldType> fieldType(int64_t id) override;
    std::optional<dto::FieldType> createFieldType(const dto::FieldType& fieldType) override;
    std::optional<dto::FieldType> updateFieldType(const dto::FieldType& fieldType) override;
    bool deleteFieldType(int64_t id) override;
    std::vector<dto::FieldType> fieldTypesByItemType(int64_t itemTypeId) override;

private:
    std::shared_ptr<repositories::IFieldTypeRepository> m_fieldTypeRepo;
};

} // namespace services
} // namespace server
