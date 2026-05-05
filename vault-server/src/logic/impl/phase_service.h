#pragma once

#include <memory>

#include "logic/iphase_service.h"
#include "repo/phase_repository.h"

namespace server
{
namespace services
{

/**
 * @brief Реализация сервиса для управления фазами проектов.
 */
class PhaseService final : public IPhaseService
{
public:
    explicit PhaseService(std::shared_ptr<repositories::IPhaseRepository> phaseRepo);

    PhasesPage phases(
        int page,
        int pageSize,
        std::optional<int64_t> projectId,
        std::optional<bool> isArchive
    ) override;
    std::optional<dto::Phase> phase(int64_t id) override;
    std::optional<dto::Phase> createPhase(const dto::Phase& phase) override;
    std::optional<dto::Phase> updatePhase(const dto::Phase& phase) override;
    bool archivePhase(int64_t id) override;
    bool restorePhase(int64_t id) override;

private:
    std::shared_ptr<repositories::IPhaseRepository> m_phaseRepo;
};

} // namespace services
} // namespace server
