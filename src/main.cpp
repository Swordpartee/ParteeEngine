#include "engine/core/Engine.hpp"

#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"

#include "engine/core/modules/BehaviorModule.hpp"
#include "engine/rendering/core/RenderModule.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"

#include "engine/core/entities/BehaviorComponent.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"
#include "engine/rendering/renderables/RenderQuad.hpp"

#include <iostream>

using namespace parteeengine;

int main() {
    Engine engine;

    engine.createModule<BehaviorModule>();
    engine.createModule<rendering::RenderModule<rendering::OpenGLRenderer>>()
        .registerComponent<rendering::QuadRenderCommand>(
            rendering::RenderQuadComponent::gatherer(),
            rendering::RenderQuadComponent::openGLHandler()
        );

    input::InputSystem::registerDevice<input::Keyboard>();

    Entity entity = engine.createEntity();
    engine.addComponent<TransformComponent2d>(entity) 
        = {200.f, 200.f};
    engine.addComponent<rendering::RenderQuadComponent>(entity)
        = {{1.f, 0.f, 0.f, 1.f}};
    engine.addComponent<BehaviorComponent>(entity)
        = {[](Entity entity, const ModuleInput& input) {
            if (input::InputSystem::isActive(input::bindings::KeySpace)) {
                auto& transform = input.entityManager.getComponent<TransformComponent2d>(entity)->transform;
                transform.position.x += 50.f * input.dt;
            }
        }};

    engine.run();

    return 0;
}