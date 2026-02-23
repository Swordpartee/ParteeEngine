#pragma once

namespace parteeengine::rendering {
    template<>
    struct RenderContext<OpenGLRenderer> {
        HDC hdc;
        HGLRC hglrc;
    };
}