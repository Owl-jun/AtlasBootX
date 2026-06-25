#include "core/UpdateManager.hpp"

#include "core/Image.hpp"
#include "interfaces/IFlash.hpp"

namespace atlasboot {

UpdateManager::UpdateManager(IFlash& flash, const MemoryLayout& layout, BootMetadata& metadata)
    : flash_(flash)
    , layout_(layout)
    , metadata_(metadata)
{
}

UpdateResult UpdateManager::InstallImage(const std::vector<std::uint8_t>& image)
{
    ImageHeader header;
    std::vector<std::uint8_t> payload;
    if (!ParseImage(image, header, payload)) {
        return UpdateResult::InvalidImage;
    }

    if (payload.size() > layout_.slot_size) {
        return UpdateResult::ImageTooLarge;
    }

    const Slot target_slot = OtherSlot(metadata_.confirmed_slot);
    const std::uint32_t target_address = SlotAddress(layout_, target_slot);

    if (!flash_.Erase(target_address, layout_.slot_size)) {
        return UpdateResult::FlashError;
    }

    if (!flash_.Write(target_address, payload.data(), static_cast<std::uint32_t>(payload.size()))) {
        return UpdateResult::FlashError;
    }

    metadata_.pending_slot = target_slot;
    metadata_.has_pending = true;
    metadata_.pending_boot_attempts = 0;
    return UpdateResult::Success;
}

} // namespace atlasboot
