#pragma once

#include <cstdint>

namespace atlasboot {

class IApplicationLauncher {
public:
    virtual ~IApplicationLauncher() = default;

    virtual bool JumpTo(std::uint32_t address) = 0;
};

} // namespace atlasboot
