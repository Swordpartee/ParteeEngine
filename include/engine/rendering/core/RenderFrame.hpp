#pragma once

#include "engine/rendering/core/RenderComponent.hpp"

#include <vector>
#include <memory>

namespace parteeengine::rendering {

    struct RenderFrame {
        std::vector<std::unique_ptr<RenderComponent>> commands;  // Store copies of components
    };

} // namespace parteeengine::rendering