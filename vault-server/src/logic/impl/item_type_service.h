#pragma once

#include <memory>

#include "logic/iitem_type_service.h"
#include "repo/item_type_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса для управления типами элементов.
 */
class ItemTypeService final : public IItemTypeService
{
public:
    explicit ItemTypeService(std::shared_ptr<repositories::IItemTypeRepository> itemTypeRepo);

    ItemTypesPage itemTypes(
        int page,
        int pageSize,
        std::optional<int64_t> workflowId,
        std::optional<std::string> kind,
        const std::string& searchCaption
    ) override;

    std::optional<dto::ItemType> itemType(int64_t id) override;
    std::optional<dto::ItemType> createItemType(const dto::ItemType& itemType) override;
    std::optional<dto::ItemType> updateItemType(const dto::ItemType& itemType) override;
    bool deleteItemType(int64_t id) override;
    std::vector<dto::ItemType> itemTypesByWorkflow(int64_t workflowId) override;

private:
    std::shared_ptr<repositories::IItemTypeRepository> m_itemTypeRepo;
};

} // namespace services
} // namespace server
