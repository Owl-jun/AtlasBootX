#pragma once

#include "interfaces/IApplicationLauncher.hpp"

namespace atlasboot::ports {

class TargetApplicationLauncher final : public IApplicationLauncher {
public:
    bool JumpTo(std::uint32_t address) override;
};

} // namespace atlasboot::ports
