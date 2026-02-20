#pragma once

#include <vector>

namespace parteeengine::rendering {

    template<typename CommandType>
    concept IsRenderCommand = std::derived_from<CommandType, VirtualRenderCommand>;

    struct VirtualRenderCommandBucket {};

    template<IsRenderCommand CommandType>
    struct RenderCommandBucket : public VirtualRenderCommandBucket {
        
        std::vector<CommandType> commands;

    };

} // namespace parteeengine::rendering