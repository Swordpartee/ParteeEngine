#pragma once

#include "engine/rendering/core/RenderCommand.hpp"

#include <vector>
#include <memory>

namespace ParteeEngine::Rendering {

    struct RenderFrame {
        std::vector<std::unique_ptr<RenderCommand>> commands;
    };

} // namespace ParteeEngine