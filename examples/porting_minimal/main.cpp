#include "core/BootManager.hpp"
#include "core/Image.hpp"
#include "core/UpdateManager.hpp"
#include "ports/native_sim/NativeSimPort.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

using namespace atlasboot;

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
    ports::NativeSimPort port;
    if (!port.Init()) {
        std::cerr << "port init failed\n";
        return 1;
    }

    std::cout << "[port] flash/application launcher/memory layout ready\n";

    const std::vector<std::uint8_t> app_payload = {
        0x10, 0x20, 0x30, 0x40,
        0x50, 0x60, 0x70, 0x80,
    };
    const std::vector<std::uint8_t> app_image = BuildImage(1, app_payload);
    std::cout << "[update] built firmware image, size=" << app_image.size() << " bytes\n";

    UpdateManager updater(port.Flash(), port.Layout(), port.Metadata());
    const UpdateResult update_result = updater.InstallImage(app_image);
    if (update_result != UpdateResult::Success) {
        std::cerr << "[update] install failed\n";
        return 1;
    }
    std::cout << "[update] installed image to inactive slot and marked it pending\n";

    BootManager boot(port.Metadata());
    const BootAction first_boot = boot.Decide();
    const std::uint32_t first_boot_address = SlotAddress(port.Layout(), port.Metadata().active_slot);
    port.ApplicationLauncher().JumpTo(first_boot_address);

    std::cout << "[boot] first decision=" << ToString(first_boot)
              << ", jump_address=0x" << std::hex << port.LastJumpAddress() << std::dec << "\n";

    boot.ConfirmPendingImage();
    std::cout << "[app] application confirmed the pending image\n";

    const BootAction next_boot = boot.Decide();
    const std::uint32_t next_boot_address = SlotAddress(port.Layout(), port.Metadata().active_slot);
    port.ApplicationLauncher().JumpTo(next_boot_address);

    std::cout << "[boot] next decision=" << ToString(next_boot)
              << ", jump_address=0x" << std::hex << port.LastJumpAddress() << std::dec << "\n";

    return first_boot == BootAction::BootPending && next_boot == BootAction::BootActive ? 0 : 1;
}
