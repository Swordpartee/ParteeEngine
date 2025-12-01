#include "engine/modules/RenderModule2d.hpp"

#include <GL/gl.h>
#include <windows.h>

#include "engine/entities/components/RenderComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"

namespace ParteeEngine {

    struct RenderData;

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
            auto *renderComp = entity.getComponent<RenderComponent>();
            if (!renderComp) continue;

            const RenderData &data = renderComp->getRenderData();
            if (data.sprite.textureID == 0) continue; // Skip if no texture
            
            // Get position from transform component
            auto *transformComp = entity.getComponent<TransformComponent>();
            if (!transformComp) continue;
            
            const auto &position = transformComp->getPosition();
            
            // Draw a square at entity's position
            glBegin(GL_QUADS);
            glVertex2f(position.x, position.y);
            glVertex2f(position.x + data.sprite.width, position.y);
            glVertex2f(position.x + data.sprite.width, position.y + data.sprite.height);
            glVertex2f(position.x, position.y + data.sprite.height);
            glEnd();

        }

        // Swap buffers (show what we drew)
        SwapBuffers(window->getHDC());
    }

} // namespace ParteeEngine