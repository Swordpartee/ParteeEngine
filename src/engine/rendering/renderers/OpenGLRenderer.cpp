#include "engine/rendering/renderers/OpenGLRenderer.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <cmath>
#include <algorithm>

namespace parteeengine::rendering {

    bool OpenGLRenderer::initialize(IWindow& window) {
        windowPtr = &window;
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

        HDC hdc = static_cast<HDC>(window.getNativeContext().deviceContext);
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);

        
        HGLRC hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);

        // Setup viewport
        RECT rect;
        GetClientRect(static_cast<HWND>(window.getNativeContext().windowHandle), &rect);
        glViewport(0, 0, rect.right, rect.bottom);

        // Setup 2D orthographic projection for screen space rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Map screen coordinates directly: (0,0) at top-left to (width, height) at bottom-right
        float left = 0.0f;
        float right = (float)rect.right;
        float bottomCoord = (float)rect.bottom;
        float top = 0.0f;
        float nearPlane = -1.0f;
        float farPlane = 1.0f;

        // Manual orthographic projection matrix
        float orthoMatrix[16] = {
            2.0f / (right - left), 0, 0, 0,
            0, 2.0f / (top - bottomCoord), 0, 0,
            0, 0, -2.0f / (farPlane - nearPlane), 0,
            -(right + left) / (right - left), -(top + bottomCoord) / (top - bottomCoord), -(farPlane + nearPlane) / (farPlane - nearPlane), 1
        };
        
        glLoadMatrixf(orthoMatrix);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Enable 2D rendering features
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        return true;
    }

    bool OpenGLRenderer::render(RenderFrame& frame) {
        // Clear the screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& cmdPtr : frame.commands) {
            if (cmdPtr->getType() == typeid(QuadRenderComponent)) {
                const QuadRenderComponent* quadComponent = static_cast<const QuadRenderComponent*>(cmdPtr);
                const Quad& quad = *quadComponent;

                glColor4f(quad.color.r, quad.color.g, quad.color.b, quad.color.a);
                glBegin(GL_QUADS);
                glVertex3f(quad.x, quad.y, 0.0f);
                glVertex3f(quad.x + quad.width, quad.y, 0.0f);
                glVertex3f(quad.x + quad.width, quad.y + quad.height, 0.0f);
                glVertex3f(quad.x, quad.y + quad.height, 0.0f);
                glEnd();
            }
        }

        return true;
    }

} // namespace parteeengine::rendering
