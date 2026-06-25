#pragma once

#include "core/BootTypes.hpp"
#include "ports/template/TargetApplicationLauncher.hpp"
#include "ports/template/TargetFlash.hpp"

namespace atlasboot::ports {

class TargetPort {
public:
    TargetPort();

    bool Init();

    IFlash& Flash();
    IApplicationLauncher& ApplicationLauncher();

    const MemoryLayout& Layout() const;
    BootMetadata& Metadata();

private:
    TargetFlash flash_;
    TargetApplicationLauncher launcher_;
    MemoryLayout layout_;
    BootMetadata metadata_;
};

} // namespace atlasboot::ports
