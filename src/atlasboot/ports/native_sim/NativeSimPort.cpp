#include "ports/native_sim/NativeSimPort.hpp"

#include "ports/native_sim/NativeSimMemoryLayout.hpp"

namespace atlasboot::ports {

NativeSimPort::NativeSimPort()
    : flash_(NativeSimMemoryLayout::kFlashSize, NativeSimMemoryLayout::kSectorSize)
    , layout_(NativeSimMemoryLayout::Make())
{
    metadata_.active_slot = Slot::A;
    metadata_.confirmed_slot = Slot::A;
    metadata_.pending_slot = Slot::A;
    metadata_.max_boot_attempts = 1;
}

bool NativeSimPort::Init()
{
    return flash_.Init();
}

IFlash& NativeSimPort::Flash()
{
    return flash_;
}

IApplicationLauncher& NativeSimPort::ApplicationLauncher()
{
    return launcher_;
}

const MemoryLayout& NativeSimPort::Layout() const
{
    return layout_;
}

BootMetadata& NativeSimPort::Metadata()
{
    return metadata_;
}

std::uint32_t NativeSimPort::LastJumpAddress() const
{
    return launcher_.LastJumpAddress();
}

} // namespace atlasboot::ports
