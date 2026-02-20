#include "engine/rendering/core/RenderModule2d.hpp"

#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"
#include "engine/rendering/core/RenderCommandBucket.hpp"

namespace parteeengine::rendering {

    RenderModule2d::RenderModule2d() : window(IWindow::createPlatformWindow()) {};

    bool RenderModule2d::initialize([[maybe_unused]]const ModuleInput& input) {
        window->create();
        renderer->initialize(*window);
        return true;
    };

    bool RenderModule2d::update(const ModuleInput& input) {
        // Poll window events
        window->pollEvents();

        // Clear previous frame's commands
        currentFrame.clearAllCommandBuckets();

        // Gather render commands from entities with RenderComponent
        // TODO: gather commands


        // Send the frame to the renderer
        renderer->render(currentFrame);

        return true; // Return false to signal engine shutdown
    }

} // namespace parteeengine::rendering