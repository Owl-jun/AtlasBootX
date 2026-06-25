#pragma once

#include "core/BootTypes.hpp"

#include <cstdint>

namespace atlasboot::ports {

struct NativeSimMemoryLayout {
    static constexpr std::uint32_t kFlashSize = 64 * 1024;
    static constexpr std::uint32_t kSectorSize = 1024;
    static constexpr std::uint32_t kSlotAAddress = 0;
    static constexpr std::uint32_t kSlotBAddress = 32 * 1024;
    static constexpr std::uint32_t kSlotSize = 32 * 1024;

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
