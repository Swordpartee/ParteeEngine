#pragma once

#include "engine/rendering/renderers/IRenderer.hpp"
#include "engine/rendering/window/IWindow.hpp"
#include "engine/rendering/core/RenderFrame.hpp"

namespace parteeengine::rendering {

    class OpenGLRenderer : public IRenderer {
    public:
        ~OpenGLRenderer() override = default;

        bool initialize(IWindow& window) override;

        bool render(RenderFrame& frame) override;

    private:
        IWindow* windowPtr = nullptr;
    };

} // namespace parteeengine::rendering