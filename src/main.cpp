#include "engine/core/Engine.hpp"

#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"

#include "engine/core/modules/BehaviorModule.hpp"

#include "engine/core/entities/BehaviorComponent.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

#include <iostream>

using namespace parteeengine;

int main() {
    Engine engine;

    engine.createModule<BehaviorModule>();

    input::InputSystem::registerDevice<input::Keyboard>();

    Entity entity = engine.createEntity();
    engine.addComponent<TransformComponent2d>(entity) 
        = {200.f, 200.f};
    engine.addComponent<BehaviorComponent>(entity)
        = {[](Entity entity, const ModuleInput& input) {
            if (input::InputSystem::isActive(input::bindings::KeySpace)) {
                auto& transform = input.entityManager.getComponent<TransformComponent2d>(entity)->transform;
                transform.position.x += 50.f * input.dt;
            }
        }};

    Entity entity2 = engine.createEntity();
    engine.addComponent<TransformComponent2d>(entity2) 
        = {{400.f, 300.f}, 0.f, {50.f, 50.f}};

    engine.run();

    return 0;
}