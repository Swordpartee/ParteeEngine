#pragma once

#include <vector>

namespace parteeengine::rendering {

    struct IRenderCommandBucket {
        virtual ~IRenderCommandBucket() = default;
    };

    template<typename CommandType>
    struct RenderCommandBucket : public IRenderCommandBucket {
        std::vector<CommandType> commands;
    };

} // namspace parteeengine::rendering 