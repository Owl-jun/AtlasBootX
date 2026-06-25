#include "ports/template/TargetFlash.hpp"

namespace atlasboot::ports {

bool TargetFlash::Init()
{
    // TODO: initialize/unlock the target flash controller if required.
    return false;
}

bool TargetFlash::Erase(std::uint32_t address, std::uint32_t size)
{
    (void)address;
    (void)size;

    // TODO:
    // - validate sector alignment
    // - erase the requested flash range
    // - wait for completion
    // - return false on any hardware error
    return false;
}

bool TargetFlash::Write(std::uint32_t address, const std::uint8_t* data, std::uint32_t size)
{
    (void)address;
    (void)data;
    (void)size;

    // TODO:
    // - validate write alignment
    // - program the requested bytes/words
    // - verify readback if the target requires it
    // - return false on any hardware error
    return false;
}

bool TargetFlash::Read(std::uint32_t address, std::uint8_t* data, std::uint32_t size)
{
    (void)address;
    (void)data;
    (void)size;

    // TODO:
    // - copy from memory-mapped flash or call the target read API
    // - return false if the address range is invalid
    return false;
}

} // namespace atlasboot::ports
