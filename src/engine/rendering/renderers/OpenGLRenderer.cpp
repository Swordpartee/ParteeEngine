#include "engine/rendering/renderers/OpenGLRenderer.hpp"

#include "engine/rendering/renderers/OpenGLRenderContext.hpp"

namespace parteeengine::rendering {
    
    bool OpenGLRenderer::initialize(IWindow& window) {
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32, // Color depth
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24, // Depth buffer
            8,  // Stencil buffer
            0, PFD_MAIN_PLANE, 0, 0, 0, 0
        };
        
        hdc = static_cast<HDC>(window.getNativeContext().deviceContext);
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);
        
        hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);

        // Setup viewport
        RECT rect;
        GetClientRect(static_cast<HWND>(window.getNativeContext().windowHandle), &rect);
        glViewport(0, 0, rect.right, rect.bottom);

        // Setup 2D orthographic projection for screen space rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, (float)rect.right, (float)rect.bottom, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Enable 2D rendering features
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        return true;
    };

    bool OpenGLRenderer::render(RenderFrame& frame, [[maybe_unused]]IWindow& window) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderContext<OpenGLRenderer> context { hdc, hglrc };

        for (auto& [type, bucket] : frame.buckets) {
            auto it = handlers.find(type);
            if (it != handlers.end())
                it->second(*bucket, context);
        }

        return true;
    };

} // namespace parteeengine::rendering
