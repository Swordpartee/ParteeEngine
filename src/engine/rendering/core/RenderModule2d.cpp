#include "engine/rendering/core/RenderModule2d.hpp"

#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

namespace parteeengine::rendering {

    RenderModule2d::RenderModule2d() : window(IWindow::createPlatformWindow()) {};

    bool RenderModule2d::initialize([[maybe_unused]]const ModuleInput& input) {
        window->create();
        renderer->initialize(*window);
        return true;
    };

    bool RenderModule2d::update(const ModuleInput& input) {
        RenderFrame frame;
        for (auto& [entity, renderComponent] : input.entityManager.getEntityComponentPairs<QuadRenderComponent>()) {
            TransformComponent2d* position = input.entityManager.getComponent<TransformComponent2d>(entity);
            if (!position) {
                continue;
            }

            frame.commands.push_back(std::make_unique<QuadRenderCommand>(position->transform, renderComponent.color));
        }

        return renderer->render(frame) && window->swapBuffers() && window->pollEvents();
    };

} // namespace parteeengine::rendering