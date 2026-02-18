#pragma once

#include "engine/rendering/core/RenderComponent.hpp"

#include <vector>
#include <memory>

namespace parteeengine::rendering {

    struct RenderFrame {
        std::vector<RenderComponent*> commands;
    };

} // namespace parteeengine::rendering