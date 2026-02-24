#pragma once

#include "engine/rendering/core/RenderFrame.hpp"
#include "engine/rendering/core/RenderModule.hpp"

namespace parteeengine::rendering {

    struct IRenderContext {};

    template<typename Renderer>
    struct RenderContext;
    class IWindow;

    class IRenderer {

        virtual bool initialize(IWindow& window) = 0;

        virtual bool render(RenderFrame& frame, IWindow& window) = 0;

    };

} // namespace parteeengine::rendering