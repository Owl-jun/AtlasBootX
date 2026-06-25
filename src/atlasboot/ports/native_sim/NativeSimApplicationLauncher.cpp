#include "ports/native_sim/NativeSimApplicationLauncher.hpp"

namespace atlasboot::ports {

bool NativeSimApplicationLauncher::JumpTo(std::uint32_t address)
{
    last_jump_address_ = address;
    return true;
}

std::uint32_t NativeSimApplicationLauncher::LastJumpAddress() const
{
    return last_jump_address_;
}

} // namespace atlasboot::ports
