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

} // namespace ParteeEngine
