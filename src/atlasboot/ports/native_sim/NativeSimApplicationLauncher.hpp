#pragma once

#include "interfaces/IApplicationLauncher.hpp"

namespace atlasboot::ports {

class NativeSimApplicationLauncher final : public IApplicationLauncher {
public:
    bool JumpTo(std::uint32_t address) override;

    std::uint32_t LastJumpAddress() const;

private:
    std::uint32_t last_jump_address_ = 0;
};

} // namespace atlasboot::ports
