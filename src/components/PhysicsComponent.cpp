#include "components/TransformComponent.hpp"
#include "Entity.hpp"

#include "components/PhysicsComponent.hpp"
#include "events/EventBus.hpp"
#include "events/Event.hpp"

namespace ParteeEngine
{
    void PhysicsComponent::requireDependencies(Entity &owner) {
        owner.ensureComponent<TransformComponent>();
    }

    void PhysicsComponent::onAttach(Entity &owner) {
        EventBus::getInstance().subscribe<CollisionEvent>([&owner](CollisionEvent e) { 
            if (e.getFirst().getID() == owner.getID() || e.getSecond().getID() == owner.getID()) {
                PhysicsComponent* physics = owner.getComponent<PhysicsComponent>();
                if (physics && physics->collisionsEnabled) {
                    physics->resetMotion();
                }
            }
        });
    }
    
    void PhysicsComponent::update(Entity& owner, float dt)
    {
        velocity += acceleration * dt;
        owner.getComponent<TransformComponent>()->translate(velocity * dt);
    }

    void PhysicsComponent::applyForce(const Vector3 &force) {
        acceleration += force;
    }

    void PhysicsComponent::applyImpulse(const Vector3 &impulse) {
        velocity += impulse;
    }

    void PhysicsComponent::setVelocity(const Vector3 &vel) {
        velocity = vel;
    }

    void PhysicsComponent::setAcceleration(const Vector3 &acc) {
        acceleration = acc;
    }

    void PhysicsComponent::resetMotion() {
        velocity = Vector3(0.0f, 0.0f, 0.0f);
        acceleration = Vector3(0.0f, 0.0f, 0.0f);
    }

}