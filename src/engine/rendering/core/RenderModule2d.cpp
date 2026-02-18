#include "engine/rendering/core/RenderModule2d.hpp"

#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

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
        for (auto& [entity, component] : input.entityManager.getEntityComponentPairs<QuadRenderComponent>()) {
            TransformComponent2d position = input.entityManager.getComponent<TransformComponent2d>(entity);
            component.position = position;
            frame.commands.emplace_back(std::make_unique<QuadRenderComponent>(component));  // Copy, don't store pointer
        }

        return renderer->render(frame) && window->swapBuffers() && window->pollEvents();
    };

} // namespace parteeengine::rendering