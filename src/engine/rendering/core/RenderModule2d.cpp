#include "engine/rendering/core/RenderModule2d.hpp"

#include "engine/rendering/renderers/OpenGLRenderer.hpp"

namespace parteeengine::rendering {

    RenderModule2d::RenderModule2d() : window(IWindow::createPlatformWindow()), renderer(std::make_unique<OpenGLRenderer>()) {

    };

    bool RenderModule2d::initialize([[maybe_unused]]const ModuleInput& input) {
        window->create();
        renderer->initialize(*window);
        return true;
    };


    bool RenderModule2d::update(const ModuleInput& input) {
        RenderFrame frame;
        for (auto& component : input.entityManager.getComponentArray<QuadRenderComponent>()) {
            frame.commands.emplace_back(&component);
        }

        return renderer->render(frame) && window->swapBuffers() && window->pollEvents();
    };

} // namespace parteeengine::rendering