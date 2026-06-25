#include "core/BootManager.hpp"
#include "core/Image.hpp"
#include "core/UpdateManager.hpp"
#include "ports/native_sim/VirtualFlash.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

using namespace atlasboot;
using atlasboot::ports::VirtualFlash;

namespace {

MemoryLayout DefaultLayout()
{
    MemoryLayout layout;
    layout.slot_a_address = 0;
    layout.slot_b_address = 1024;
    layout.slot_size = 1024;
    return layout;
}

void ValidUpdateBootsPendingThenConfirms()
{
    VirtualFlash flash(2048, 256);
    assert(flash.Init());

    BootMetadata metadata;
    metadata.confirmed_slot = Slot::A;
    metadata.active_slot = Slot::A;
    metadata.max_boot_attempts = 1;

    const auto image = BuildImage(1, {0x01, 0x02, 0x03, 0x04});

    const MemoryLayout layout = DefaultLayout();
    UpdateManager updater(flash, layout, metadata);
    assert(updater.InstallImage(image) == UpdateResult::Success);
    assert(metadata.has_pending);
    assert(metadata.pending_slot == Slot::B);

    BootManager boot(metadata);
    assert(boot.Decide() == BootAction::BootPending);
    assert(metadata.active_slot == Slot::B);

    boot.ConfirmPendingImage();
    assert(!metadata.has_pending);
    assert(metadata.confirmed_slot == Slot::B);
    assert(boot.Decide() == BootAction::BootActive);
}

void InvalidImageIsRejected()
{
    VirtualFlash flash(2048, 256);
    assert(flash.Init());

    BootMetadata metadata;
    const std::vector<std::uint8_t> invalid_image = {0x00, 0x01, 0x02};

    const MemoryLayout layout = DefaultLayout();
    UpdateManager updater(flash, layout, metadata);
    assert(updater.InstallImage(invalid_image) == UpdateResult::InvalidImage);
    assert(!metadata.has_pending);
}

void RollbackAfterUnconfirmedPendingBoot()
{
    BootMetadata metadata;
    metadata.confirmed_slot = Slot::A;
    metadata.active_slot = Slot::A;
    metadata.pending_slot = Slot::B;
    metadata.has_pending = true;
    metadata.max_boot_attempts = 1;

    BootManager boot(metadata);
    assert(boot.Decide() == BootAction::BootPending);
    assert(boot.Decide() == BootAction::RollbackToConfirmed);
    assert(metadata.active_slot == Slot::A);
    assert(!metadata.has_pending);
}

void VirtualFlashRequiresEraseBeforeSettingBits()
{
    VirtualFlash flash(256, 64);
    assert(flash.Init());

    const std::uint8_t first = 0x00;
    const std::uint8_t second = 0xFF;

    assert(flash.Erase(0, 64));
    assert(flash.Write(0, &first, 1));
    assert(!flash.Write(0, &second, 1));
}

} // namespace

int main()
{
    ValidUpdateBootsPendingThenConfirms();
    InvalidImageIsRejected();
    RollbackAfterUnconfirmedPendingBoot();
    VirtualFlashRequiresEraseBeforeSettingBits();
    return 0;
}
