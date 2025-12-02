#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"

namespace ParteeEngine
{
    class Vector3;

    class RigidBodyComponent : public Component {
    friend class Entity;

    public:
        void ensureDependencies() override {
            owner.ensureComponent<TransformComponent>();
        };

        float getMass() const { return mass; }
        void setMass(float m) { mass = m; }

        const Vector3 &getVelocity() const { return velocity; }
        void setVelocity(const Vector3 &vel) { velocity = vel; }

    protected:
        using Component::Component;

    private:
        float mass = 1.0f;
        Vector3 velocity{0.f, 0.f, 0.f};

    };

    template <>
    struct ComponentTraits<RigidBodyComponent>
    {
        static constexpr bool unique = true;
        static constexpr ComponentCategory categories = ComponentCategory::Physics;
    };
    
} // namespace ParteeEngine