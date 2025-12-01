#include "engine/modules/RenderModule2d.hpp"

#include <GL/gl.h>
#include <windows.h>

namespace ParteeEngine {

    void RenderModule2d::initialize(const ModuleInputs &inputs) {
        window = &inputs.window;

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32, // Color depth
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24, // Depth buffer
            8,  // Stencil buffer
            0, PFD_MAIN_PLANE, 0, 0, 0, 0};

        HDC hdc = window->getHDC();
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);

        HGLRC hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);

        // Setup viewport
        RECT rect;
        GetClientRect(window->getHWND(), &rect);
        glViewport(0, 0, rect.right, rect.bottom);

        // Setup 2D orthographic projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, rect.right, rect.bottom, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void RenderModule2d::update(const ModuleUpdateInputs &inputs) {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Render each entity
        for (const auto &entity : inputs.entities)
        {
            // Check if entity has a renderable component
            // Example: drawing a simple rectangle
            glBegin(GL_QUADS);
            glColor3f(1.0f, 0.0f, 0.0f); // Red
            glVertex2f(100.0f, 100.0f);
            glVertex2f(200.0f, 100.0f);
            glVertex2f(200.0f, 200.0f);
            glVertex2f(100.0f, 200.0f);
            glEnd();
        }

        // Swap buffers (show what we drew)
        SwapBuffers(window->getHDC());
    }

} // namespace ParteeEngine