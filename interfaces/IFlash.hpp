#pragma once

#include <cstdint>

namespace atlasboot {

class IFlash {
public:
    virtual ~IFlash() = default;

    virtual bool Init() = 0;
    virtual bool Erase(std::uint32_t address, std::uint32_t size) = 0;
    virtual bool Write(std::uint32_t address, const std::uint8_t* data, std::uint32_t size) = 0;
    virtual bool Read(std::uint32_t address, std::uint8_t* data, std::uint32_t size) = 0;
};

} // namespace atlasboot
