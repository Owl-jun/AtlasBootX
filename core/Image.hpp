#pragma once

#include <cstdint>
#include <vector>

namespace atlasboot {

constexpr std::uint32_t kImageMagic = 0x41544254; // ATBT

struct ImageHeader {
    std::uint32_t magic = kImageMagic;
    std::uint32_t version = 0;
    std::uint32_t payload_size = 0;
    std::uint32_t checksum = 0;
};

std::uint32_t CalculateChecksum(const std::uint8_t* data, std::uint32_t size);
std::vector<std::uint8_t> BuildImage(std::uint32_t version, const std::vector<std::uint8_t>& payload);
bool ParseImage(const std::vector<std::uint8_t>& image, ImageHeader& header, std::vector<std::uint8_t>& payload);

} // namespace atlasboot
