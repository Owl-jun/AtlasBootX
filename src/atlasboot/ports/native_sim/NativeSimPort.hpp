#pragma once

#include "core/BootTypes.hpp"
#include "ports/native_sim/NativeSimApplicationLauncher.hpp"
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
    VirtualFlash flash_;
    NativeSimApplicationLauncher launcher_;
    MemoryLayout layout_;
    BootMetadata metadata_;
};

} // namespace atlasboot::ports
