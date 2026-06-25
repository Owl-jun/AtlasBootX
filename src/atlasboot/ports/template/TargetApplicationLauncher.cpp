#include "ports/template/TargetApplicationLauncher.hpp"

namespace atlasboot::ports {

bool TargetApplicationLauncher::JumpTo(std::uint32_t address)
{
    (void)address;

    // TODO:
    // - disable interrupts
    // - stop SysTick/timers used by the bootloader
    // - deinitialize peripherals if required
    // - set vector table base if the MCU supports it
    // - load the application stack pointer
    // - branch to the application reset handler
    return false;
}

} // namespace atlasboot::ports
