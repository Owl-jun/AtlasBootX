#pragma once

#include "core/BootTypes.hpp"
#include "interfaces/IApplicationLauncher.hpp"
#include "ports/native_sim/VirtualFlash.hpp"

#include <cstdint>

namespace atlasboot::ports {

class NativeSimPort {
public:
    NativeSimPort();

    bool Init();

    IFlash& Flash();
    IApplicationLauncher& ApplicationLauncher();

    const MemoryLayout& Layout() const;
    BootMetadata& Metadata();

    std::uint32_t LastJumpAddress() const;

private:
    class Launcher final : public IApplicationLauncher {
    public:
        bool JumpTo(std::uint32_t address) override;
        std::uint32_t LastJumpAddress() const;

    private:
        std::uint32_t last_jump_address_ = 0;
    };

    VirtualFlash flash_;
    Launcher launcher_;
    MemoryLayout layout_;
    BootMetadata metadata_;
};

} // namespace atlasboot::ports
