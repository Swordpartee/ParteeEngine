#pragma once

#include "engine/rendering/core/RenderFrame.hpp"

namespace parteeengine::rendering {

    class IWindow;

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool initialize(IWindow& window) = 0;

        virtual bool render(RenderFrame& frame) = 0;

    protected:
        IRenderer() = default;
    };
    
} // namespace parteeengine::rendering