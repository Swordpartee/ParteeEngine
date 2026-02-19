#include "engine/core/Engine.hpp"

#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"

#include "engine/core/modules/BehaviorModule.hpp"
#include "engine/rendering/core/RenderModule2d.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"


#include "engine/core/entities/BehaviorComponent.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"
#include "engine/rendering/core/RenderComponent.hpp"

#include <iostream>

using namespace parteeengine;

int main() {
    Engine engine;

    engine.createModule<rendering::RenderModule2d>()
        .config<rendering::OpenGLRenderer>({800, 600, "ParteeEngine"});
    engine.createModule<BehaviorModule>();

    input::InputSystem::registerDevice<input::Keyboard>();

    Entity entity = engine.createEntity();
    engine.addComponent<TransformComponent2d>(entity) 
        = {200.f, 200.f};
    engine.addComponent<rendering::QuadRenderComponent>(entity) 
        = {{255, 0, 0}};
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