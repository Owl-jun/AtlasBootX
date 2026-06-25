#pragma once

#include "core/BootTypes.hpp"

#include <cstdint>

namespace atlasboot::ports {

struct TargetMemoryLayout {
    // Replace these addresses with the target MCU flash map.
    static constexpr std::uint32_t kSlotAAddress = 0x00000000;
    static constexpr std::uint32_t kSlotBAddress = 0x00008000;
    static constexpr std::uint32_t kSlotSize = 0x00008000;

    static MemoryLayout Make()
    {
        MemoryLayout layout;
        layout.slot_a_address = kSlotAAddress;
        layout.slot_b_address = kSlotBAddress;
        layout.slot_size = kSlotSize;
        return layout;
    }
};

} // namespace atlasboot::ports
