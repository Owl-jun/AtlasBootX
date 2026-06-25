#include "ports/native_sim/NativeSimPort.hpp"

namespace atlasboot::ports {

namespace {

constexpr std::uint32_t kFlashSize = 64 * 1024;
constexpr std::uint32_t kSectorSize = 1024;
constexpr std::uint32_t kSlotSize = 32 * 1024;

} // namespace

NativeSimPort::NativeSimPort()
    : flash_(kFlashSize, kSectorSize)
{
    layout_.slot_a_address = 0;
    layout_.slot_b_address = kSlotSize;
    layout_.slot_size = kSlotSize;

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

bool NativeSimPort::Launcher::JumpTo(std::uint32_t address)
{
    last_jump_address_ = address;
    return true;
}

std::uint32_t NativeSimPort::Launcher::LastJumpAddress() const
{
    return last_jump_address_;
}

} // namespace atlasboot::ports
