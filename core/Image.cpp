#include "core/Image.hpp"

#include <cstring>

namespace atlasboot {

std::uint32_t CalculateChecksum(const std::uint8_t* data, std::uint32_t size)
{
    std::uint32_t checksum = 0;
    for (std::uint32_t i = 0; i < size; ++i) {
        checksum = (checksum * 16777619u) ^ data[i];
    }
    return checksum;
}

std::vector<std::uint8_t> BuildImage(std::uint32_t version, const std::vector<std::uint8_t>& payload)
{
    ImageHeader header;
    header.version = version;
    header.payload_size = static_cast<std::uint32_t>(payload.size());
    header.checksum = CalculateChecksum(payload.data(), header.payload_size);

    std::vector<std::uint8_t> image(sizeof(ImageHeader) + payload.size());
    std::memcpy(image.data(), &header, sizeof(ImageHeader));
    std::memcpy(image.data() + sizeof(ImageHeader), payload.data(), payload.size());
    return image;
}

bool ParseImage(const std::vector<std::uint8_t>& image, ImageHeader& header, std::vector<std::uint8_t>& payload)
{
    if (image.size() < sizeof(ImageHeader)) {
        return false;
    }

    std::memcpy(&header, image.data(), sizeof(ImageHeader));
    if (header.magic != kImageMagic) {
        return false;
    }

    if (image.size() != sizeof(ImageHeader) + header.payload_size) {
        return false;
    }

    payload.assign(image.begin() + sizeof(ImageHeader), image.end());
    return CalculateChecksum(payload.data(), header.payload_size) == header.checksum;
}

} // namespace atlasboot
