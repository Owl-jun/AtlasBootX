#include "core/BootManager.hpp"

namespace atlasboot {

BootManager::BootManager(BootMetadata& metadata)
    : metadata_(metadata)
{
}

BootAction BootManager::Decide()
{
    if (!metadata_.has_pending) {
        return BootAction::BootActive;
    }

    if (metadata_.pending_boot_attempts < metadata_.max_boot_attempts) {
        metadata_.active_slot = metadata_.pending_slot;
        ++metadata_.pending_boot_attempts;
        return BootAction::BootPending;
    }

    metadata_.active_slot = metadata_.confirmed_slot;
    metadata_.has_pending = false;
    metadata_.pending_boot_attempts = 0;
    return BootAction::RollbackToConfirmed;
}

void BootManager::ConfirmPendingImage()
{
    if (!metadata_.has_pending) {
        return;
    }

    metadata_.confirmed_slot = metadata_.pending_slot;
    metadata_.active_slot = metadata_.pending_slot;
    metadata_.has_pending = false;
    metadata_.pending_boot_attempts = 0;
}

} // namespace atlasboot
