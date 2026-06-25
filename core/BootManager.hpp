#pragma once

#include "core/BootTypes.hpp"

namespace atlasboot {

class BootManager {
public:
    explicit BootManager(BootMetadata& metadata);

    BootAction Decide();
    void ConfirmPendingImage();

private:
    BootMetadata& metadata_;
};

} // namespace atlasboot
