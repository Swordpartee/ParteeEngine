#pragma once

#include "engine/rendering/core/RenderCommand.hpp"

#include <vector>
#include <memory>

namespace parteeengine::rendering {

    struct RenderFrame {
        std::vector<std::unique_ptr<VirtualRenderCommand>> commands;  // Store copies of components
    };

} // namespace parteeengine::rendering