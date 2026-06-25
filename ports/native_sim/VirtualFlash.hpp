#pragma once

#include "interfaces/IFlash.hpp"

#include <cstdint>
#include <vector>

namespace atlasboot::ports {

class VirtualFlash final : public IFlash {
public:
    VirtualFlash(std::uint32_t size, std::uint32_t sector_size);

    bool Init() override;
    bool Erase(std::uint32_t address, std::uint32_t size) override;
    bool Write(std::uint32_t address, const std::uint8_t* data, std::uint32_t size) override;
    bool Read(std::uint32_t address, std::uint8_t* data, std::uint32_t size) override;

    void InjectNextEraseFailure();
    void InjectNextWriteFailure();
    const std::vector<std::uint8_t>& Data() const;

private:
    bool IsRangeValid(std::uint32_t address, std::uint32_t size) const;

    std::vector<std::uint8_t> memory_;
    std::uint32_t sector_size_;
    bool fail_next_erase_ = false;
    bool fail_next_write_ = false;
};

} // namespace atlasboot::ports
