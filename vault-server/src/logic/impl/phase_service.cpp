#include "common/log/log.h"

#include "phase_service.h"

namespace server
{
namespace services
{

PhaseService::PhaseService(
    std::shared_ptr<repositories::IPhaseRepository> phaseRepo
)
    : m_phaseRepo(std::move(phaseRepo))
{
    if (!m_phaseRepo)
    {
        throw std::runtime_error("PhaseRepository cannot be null");
    }
}

PhasesPage PhaseService::phases(
    int page,
    int pageSize,
    std::optional<int64_t> projectId,
    std::optional<bool> isArchive
)
{
    if (page < 1)
        page = 1;
    if (pageSize < 1)
        pageSize = 20;

    auto [phaseList, total] = m_phaseRepo->findAll(
        page,
        pageSize,
        projectId,
        isArchive
    );
    return { phaseList, total };
}

std::optional<dto::Phase> PhaseService::phase(int64_t id)
{
    return m_phaseRepo->findById(id);
}

std::optional<dto::Phase> PhaseService::createPhase(const dto::Phase& phase)
{
    if (!phase.caption.has_value() || phase.caption->empty())
    {
        LOG_WARN << "createPhase: название фазы не может быть пустым";
        return std::nullopt;
    }

    if (!phase.projectId.has_value())
    {
        LOG_WARN << "createPhase: не указан projectId";
        return std::nullopt;
    }

    const int64_t newId = m_phaseRepo->create(phase);
    if (newId <= 0)
    {
        LOG_ERROR << "createPhase: ошибка при создании фазы в репозитории";
        return std::nullopt;
    }

    LOG_INFO << "Создана новая фаза с id=" << newId;
    return m_phaseRepo->findById(newId);
}

std::optional<dto::Phase> PhaseService::updatePhase(const dto::Phase& phase)
{
    if (!phase.id.has_value())
    {
        LOG_WARN << "updatePhase: отсутствует ID фазы";
        return std::nullopt;
    }

    if (!m_phaseRepo->exists(phase.id.value()))
    {
        LOG_WARN << "updatePhase: фаза с id=" << phase.id.value() << " не найдена";
        return std::nullopt;
    }

    if (!m_phaseRepo->update(phase))
    {
        LOG_ERROR << "updatePhase: ошибка при обновлении фазы в репозитории";
        return std::nullopt;
    }

    LOG_INFO << "Фаза с id=" << phase.id.value() << " успешно обновлена";
    return m_phaseRepo->findById(phase.id.value());
}

bool PhaseService::archivePhase(int64_t id)
{
    if (!m_phaseRepo->exists(id))
    {
        LOG_WARN << "archivePhase: фаза с id=" << id << " не найдена";
        return false;
    }

    if (m_phaseRepo->archive(id))
    {
        LOG_INFO << "Фаза с id=" << id << " успешно архивирована";
        return true;
    }

    LOG_ERROR << "archivePhase: ошибка при архивации фазы с id=" << id;
    return false;
}

bool PhaseService::restorePhase(int64_t id)
{
    if (!m_phaseRepo->exists(id))
    {
        LOG_WARN << "restorePhase: фаза с id=" << id << " не найдена";
        return false;
    }

    if (m_phaseRepo->restore(id))
    {
        LOG_INFO << "Фаза с id=" << id << " успешно восстановлена";
        return true;
    }

    LOG_ERROR << "restorePhase: ошибка при восстановлении фазы с id=" << id;
    return false;
}

} // namespace services
} // namespace server
