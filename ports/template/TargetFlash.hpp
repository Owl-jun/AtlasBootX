#pragma once

#include "interfaces/IFlash.hpp"

namespace atlasboot::ports {

class TargetFlash final : public IFlash {
public:
    bool Init() override;
    bool Erase(std::uint32_t address, std::uint32_t size) override;
    bool Write(std::uint32_t address, const std::uint8_t* data, std::uint32_t size) override;
    bool Read(std::uint32_t address, std::uint8_t* data, std::uint32_t size) override;
};

} // namespace atlasboot::ports
