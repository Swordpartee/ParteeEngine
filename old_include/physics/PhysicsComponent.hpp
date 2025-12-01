#pragma once

#include "Component.hpp"
#include "Vector3.hpp"
#include "events/Event.hpp"
#include <iostream>
#include <typeindex>

namespace ParteeEngine
{
    class Entity;             // Forward declaration
    class TransformComponent; // Forward declaration

    class PhysicsComponent : public Component {
        public:
            void requireDependencies(Entity &owner) override;

            void onAttach(Entity &owner) override;

            void updatePhysics(Entity& owner, float dt);

            void applyForce(const Vector3 &force);

            void setVelocity(const Vector3 &vel);

            Vector3 getVelocity() const {
                return velocity;
            }

            void setAcceleration(const Vector3 &acc);
            Vector3 getAcceleration() const {
                return acceleration;
            }

            void applyImpulse(const Vector3 &impulse);

            void resetMotion();

            void setCollisionsEnabled(const bool enabled) {
                collisionsEnabled = enabled;
            }

            PhysicsComponent() : velocity(0.0f, 0.0f, 0.0f), acceleration(0.0f, 0.0f, 0.0f) {}

        private:
            Vector3 velocity;
            Vector3 acceleration;

            bool collisionsEnabled = true;
    };
}