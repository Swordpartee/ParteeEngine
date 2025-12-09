#include "engine/modules/RenderModule2d.hpp"

#include "engine/entities/components/RenderComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/entities/Entity.hpp"

#include <GL/gl.h>
#include <windows.h>
#include <cmath>

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

        // Setup 2D orthographic projection with sub-pixel precision
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // Use floating-point orthographic bounds to allow smooth sub-pixel rendering
        glOrtho(0.0, (double)rect.right, (double)rect.bottom, 0.0, -1000.0, 1000.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Enable smooth rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
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

            // Get position and rotation from transform component
            auto *transformComp = entity.getComponent<TransformComponent>();
            if (!transformComp) continue;
            
            const Vector3 &position = transformComp->getPosition();
            const Vector3 &normal = transformComp->getRotation();  // Use rotation as normal vector

            // Get render data
            const RenderData &data = renderComp->getRenderData();
            
            // Determine square size (use sprite dimensions if available, otherwise default)
            float halfWidth = transformComp->getScale().x * 0.5f;
            float halfHeight = transformComp->getScale().y * 0.5f;
            
            Vector3 normalizedNormal = normal.normalize();
            
            // Default forward direction (Z-axis)
            Vector3 defaultForward(0.0f, 0.0f, 1.0f);
            
            // Calculate rotation axis (cross product of default forward and target normal)
            Vector3 rotAxis = defaultForward.cross(normalizedNormal);
            float rotAxisLen = rotAxis.length();
            
            // Set up model transformation
            glPushMatrix();
            
            // Translate to position
            glTranslatef(position.x, position.y, position.z);
            
            // If the normal is not parallel to the default forward direction, rotate to face it
            if (rotAxisLen > 0.001f) {
                // Calculate rotation angle using dot product
                float dotProduct = defaultForward.dot(normalizedNormal);
                dotProduct = (dotProduct > 1.0f) ? 1.0f : (dotProduct < -1.0f) ? -1.0f : dotProduct;
                float angle = acos(dotProduct) * 180.0f / 3.14159265f;  // Convert to degrees
                
                // Normalize rotation axis
                rotAxis = rotAxis.normalize();
                
                // Apply rotation
                glRotatef(angle, rotAxis.x, rotAxis.y, rotAxis.z);
            }

            // Draw centered square
            glBegin(GL_QUADS);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glVertex3f(-halfWidth, -halfHeight, 0.0f);
            glVertex3f(halfWidth, -halfHeight, 0.0f);
            glVertex3f(halfWidth, halfHeight, 0.0f);
            glVertex3f(-halfWidth, halfHeight, 0.0f);
            glEnd();

            // Draw wireframe outline to show all edges
            glBegin(GL_LINE_LOOP);
            glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
            glVertex3f(-halfWidth, -halfHeight, 0.0f);
            glVertex3f(halfWidth, -halfHeight, 0.0f);
            glVertex3f(halfWidth, halfHeight, 0.0f);
            glVertex3f(-halfWidth, halfHeight, 0.0f);
            glEnd();

            glPopMatrix();
        }

        // Swap buffers (show what we drew)
        SwapBuffers(window->getHDC());
    }

} // namespace ParteeEngine