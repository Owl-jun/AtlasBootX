#pragma once

#include <cstdint>

namespace atlasboot {

enum class Slot : std::uint8_t {
    A = 0,
    B = 1,
};

enum class BootAction : std::uint8_t {
    BootActive,
    BootPending,
    RollbackToConfirmed,
    StayInBootloader,
};

struct MemoryLayout {
    std::uint32_t slot_a_address = 0;
    std::uint32_t slot_b_address = 0;
    std::uint32_t slot_size = 0;
};

struct BootMetadata {
    Slot active_slot = Slot::A;
    Slot confirmed_slot = Slot::A;
    bool has_pending = false;
    Slot pending_slot = Slot::A;
    std::uint32_t pending_boot_attempts = 0;
    std::uint32_t max_boot_attempts = 1;
};

inline Slot OtherSlot(Slot slot)
{
    return slot == Slot::A ? Slot::B : Slot::A;
}

inline std::uint32_t SlotAddress(const MemoryLayout& layout, Slot slot)
{
    return slot == Slot::A ? layout.slot_a_address : layout.slot_b_address;
}

} // namespace atlasboot
