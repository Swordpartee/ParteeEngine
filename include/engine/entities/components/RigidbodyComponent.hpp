#pragma once

#include "TransformComponent.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/entities/components/Component.hpp"

namespace ParteeEngine {

    class RigidBodyComponent : public Component {
    public:
        float getMass() const { return mass; }
        void setMass(float m) { mass = m; }

        Vector3& getVelocity() { return velocity; }
        const Vector3& getVelocity() const { return velocity; }
        void setVelocity(const Vector3& vel) { velocity = vel; }

        void requireDependencies() override {
            owner->ensureComponent<TransformComponent>();
        }

    private:
        float mass = 1.0f;
        Vector3 velocity{0.f, 0.f, 0.f};
    };

} // namespace ParteeEngine