#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    class ColliderComponent: public Component {
    public:
        virtual ~ColliderComponent() = default;

        void requireDependencies() {
            owner->ensureComponent<TransformComponent>();
        }

        virtual float getBoundingSphereRadius() const = 0;
    };

    class BoxColliderComponent : public ColliderComponent {
    public:

        float getBoundingSphereRadius() const override {
            auto transform = owner->getComponent<TransformComponent>();
            auto halfExtends = transform->getScale() * 0.5f;
            float radius = halfExtends.length();
            return radius;
            // return (owner->getComponent<TransformComponent>()->getScale() * 0.5f).length();
        }
        
    };

} // namespace ParteeEngine
