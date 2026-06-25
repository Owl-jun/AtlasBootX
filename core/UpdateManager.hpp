#pragma once

#include "core/BootTypes.hpp"

#include <cstdint>
#include <vector>

namespace atlasboot {

class IFlash;

enum class UpdateResult : std::uint8_t {
    Success,
    InvalidImage,
    ImageTooLarge,
    FlashError,
};

class UpdateManager {
public:
    UpdateManager(IFlash& flash, const MemoryLayout& layout, BootMetadata& metadata);

    UpdateResult InstallImage(const std::vector<std::uint8_t>& image);

private:
    IFlash& flash_;
    const MemoryLayout& layout_;
    BootMetadata& metadata_;
};

} // namespace atlasboot
