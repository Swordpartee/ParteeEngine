#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"
#include "engine/modules/PhysicsModule.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/RenderableFactory.hpp"

int main() {

    ParteeEngine::Engine engine;

    engine.addModule<ParteeEngine::RenderModule2d>();
    engine.addModule<ParteeEngine::PhysicsModule>();

    ParteeEngine::Entity& entity = ParteeEngine::RenderableFactory::createSquare(engine);
    entity.addComponent<ParteeEngine::RigidBodyComponent>();

    entity.getComponent<ParteeEngine::TransformComponent>()->setPosition({200.0f, 150.0f, 0.0f});

    entity.getComponent<ParteeEngine::RigidBodyComponent>()->setVelocity({10.0f, 0.5f, 0.0f});

    engine.run();

    return 0;
}