#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"
#include "engine/modules/PhysicsModule.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/entities/components/ColliderComponent.hpp"
#include "engine/RenderableFactory.hpp"

int main() {

    ParteeEngine::Engine engine;

    engine.addModule<ParteeEngine::RenderModule2d>();
    engine.addModule<ParteeEngine::PhysicsModule>();

    ParteeEngine::Entity& entity = ParteeEngine::RenderableFactory::createSquare(engine);
    entity.addComponent<ParteeEngine::RigidBodyComponent>();
    entity.addComponent<ParteeEngine::BoxColliderComponent>();

    entity.getComponent<ParteeEngine::TransformComponent>()->setPosition({200.0f, 150.0f, 0.0f});
    // entity.getComponent<ParteeEngine::TransformComponent>()->setRotation({1.0f, 1.0f, 1.0f});
    entity.getComponent<ParteeEngine::TransformComponent>()->setScale({50.0f, 50.0f, 50.0f});

    entity.getComponent<ParteeEngine::RigidBodyComponent>()->setVelocity({10.0f, 0.0f, 0.0f});

    ParteeEngine::Entity& entity2 = ParteeEngine::RenderableFactory::createSquare(engine);
    entity2.addComponent<ParteeEngine::BoxColliderComponent>();

    entity2.getComponent<ParteeEngine::TransformComponent>()->setPosition({300.0f, 500.0f, 0.0f});
    entity2.getComponent<ParteeEngine::TransformComponent>()->setScale({50.0f, 50.0f, 50.0f});

    engine.run();

    return 0;
}