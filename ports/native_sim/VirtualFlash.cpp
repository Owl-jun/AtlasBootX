#include "ports/native_sim/VirtualFlash.hpp"

#include <algorithm>
#include <cstring>

namespace atlasboot::ports {

VirtualFlash::VirtualFlash(std::uint32_t size, std::uint32_t sector_size)
    : memory_(size, 0xFF)
    , sector_size_(sector_size)
{
}

bool VirtualFlash::Init()
{
    return sector_size_ != 0 && memory_.size() >= sector_size_;
}

bool VirtualFlash::Erase(std::uint32_t address, std::uint32_t size)
{
    if (fail_next_erase_) {
        fail_next_erase_ = false;
        return false;
    }

    if (!IsRangeValid(address, size)) {
        return false;
    }

    if ((address % sector_size_) != 0 || (size % sector_size_) != 0) {
        return false;
    }

    std::fill(memory_.begin() + address, memory_.begin() + address + size, 0xFF);
    return true;
}

bool VirtualFlash::Write(std::uint32_t address, const std::uint8_t* data, std::uint32_t size)
{
    if (fail_next_write_) {
        fail_next_write_ = false;
        return false;
    }

    if (data == nullptr || !IsRangeValid(address, size)) {
        return false;
    }

    for (std::uint32_t i = 0; i < size; ++i) {
        const std::uint8_t current = memory_[address + i];
        const std::uint8_t next = data[i];
        if ((current & next) != next) {
            return false;
        }
    }

    for (std::uint32_t i = 0; i < size; ++i) {
        memory_[address + i] &= data[i];
    }
    return true;
}

bool VirtualFlash::Read(std::uint32_t address, std::uint8_t* data, std::uint32_t size)
{
    if (data == nullptr || !IsRangeValid(address, size)) {
        return false;
    }

    std::memcpy(data, memory_.data() + address, size);
    return true;
}

void VirtualFlash::InjectNextEraseFailure()
{
    fail_next_erase_ = true;
}

void VirtualFlash::InjectNextWriteFailure()
{
    fail_next_write_ = true;
}

const std::vector<std::uint8_t>& VirtualFlash::Data() const
{
    return memory_;
}

bool VirtualFlash::IsRangeValid(std::uint32_t address, std::uint32_t size) const
{
    return address <= memory_.size() && size <= memory_.size() - address;
}

} // namespace atlasboot::ports
