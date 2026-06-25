#include "ports/template/TargetPort.hpp"

#include "ports/template/TargetMemoryLayout.hpp"

namespace atlasboot::ports {

TargetPort::TargetPort()
    : layout_(TargetMemoryLayout::Make())
{
    metadata_.active_slot = Slot::A;
    metadata_.confirmed_slot = Slot::A;
    metadata_.pending_slot = Slot::A;
    metadata_.max_boot_attempts = 1;
}

bool TargetPort::Init()
{
    return flash_.Init();
}

IFlash& TargetPort::Flash()
{
    return flash_;
}

IApplicationLauncher& TargetPort::ApplicationLauncher()
{
    return launcher_;
}

const MemoryLayout& TargetPort::Layout() const
{
    return layout_;
}

BootMetadata& TargetPort::Metadata()
{
    return metadata_;
}

} // namespace atlasboot::ports
