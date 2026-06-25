#include "core/BootManager.hpp"
#include "core/Image.hpp"
#include "core/UpdateManager.hpp"
#include "ports/native_sim/VirtualFlash.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

using namespace atlasboot;
using atlasboot::ports::VirtualFlash;

namespace {

const char* ToString(BootAction action)
{
    switch (action) {
    case BootAction::BootActive:
        return "BootActive";
    case BootAction::BootPending:
        return "BootPending";
    case BootAction::RollbackToConfirmed:
        return "RollbackToConfirmed";
    case BootAction::StayInBootloader:
        return "StayInBootloader";
    }
    return "Unknown";
}

} // namespace

int main()
{
    constexpr std::uint32_t kFlashSize = 64 * 1024;
    constexpr std::uint32_t kSectorSize = 1024;

    VirtualFlash flash(kFlashSize, kSectorSize);
    if (!flash.Init()) {
        std::cerr << "flash init failed\n";
        return 1;
    }

    MemoryLayout layout;
    layout.slot_a_address = 0;
    layout.slot_b_address = 32 * 1024;
    layout.slot_size = 32 * 1024;

    BootMetadata metadata;
    metadata.active_slot = Slot::A;
    metadata.confirmed_slot = Slot::A;
    metadata.max_boot_attempts = 1;

    const std::vector<std::uint8_t> payload = {
        0x10, 0x20, 0x30, 0x40,
        0x50, 0x60, 0x70, 0x80,
    };
    const std::vector<std::uint8_t> image = BuildImage(1, payload);

    UpdateManager updater(flash, layout, metadata);
    const UpdateResult result = updater.InstallImage(image);
    if (result != UpdateResult::Success) {
        std::cerr << "update install failed\n";
        return 1;
    }

    BootManager boot(metadata);
    const BootAction first_boot = boot.Decide();
    std::cout << "first boot decision: " << ToString(first_boot) << "\n";

    boot.ConfirmPendingImage();

    const BootAction next_boot = boot.Decide();
    std::cout << "next boot decision: " << ToString(next_boot) << "\n";

    return first_boot == BootAction::BootPending && next_boot == BootAction::BootActive ? 0 : 1;
}
