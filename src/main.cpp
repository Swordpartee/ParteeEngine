#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"
#include "engine/entities/components/RenderComponent.hpp"

int main() {
    ParteeEngine::Engine engine;

    engine.addModule<ParteeEngine::RenderModule2d>();

    ParteeEngine::Entity& entity = engine.createEntity();

    entity.addComponent<ParteeEngine::RenderComponent>();

    entity.getComponent<ParteeEngine::TransformComponent>()->setPosition({200.0f, 150.0f, 0.0f});

    engine.run();

    return 0;
}