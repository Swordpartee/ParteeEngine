#include "Engine.hpp"
#include "Entity.hpp"
#include "components/RenderComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
#include "events/EventBus.hpp"
#include "events/Event.hpp"
#include <utility>

int main() 
{   
    ParteeEngine::Engine engine(800, 600);

    ParteeEngine::Entity &thingy2 = engine.createEntity();

    thingy2.addComponent<ParteeEngine::RenderComponent>();

    thingy2.addComponent<ParteeEngine::ColliderComponent, ParteeEngine::ColliderType>(ParteeEngine::ColliderType::CUBE);

    ParteeEngine::Entity &thingy = engine.createEntity();

    thingy.addComponent<ParteeEngine::RenderComponent>();

    thingy.addComponent<ParteeEngine::PhysicsComponent>();
    thingy.getComponent<ParteeEngine::TransformComponent>()->setPosition(-3, -3, 0);

    thingy.addComponent<ParteeEngine::ColliderComponent, ParteeEngine::ColliderType>(ParteeEngine::ColliderType::CUBE);

    thingy.getComponent<ParteeEngine::PhysicsComponent>()->applyImpulse(ParteeEngine::Vector3(1.0f, 0.0f, -3.0f));
    thingy.getComponent<ParteeEngine::PhysicsComponent>()->applyForce(ParteeEngine::Vector3(0.0f, 1.2f, 0.0f));

    ParteeEngine::EventBus::getInstance().subscribe<ParteeEngine::CollisionEvent>([](ParteeEngine::CollisionEvent e)
        { std::printf("Colliding! %d\n", e.getFirst().getID()); std::fflush(stdout); });

    engine.start();

    return 0;
} 