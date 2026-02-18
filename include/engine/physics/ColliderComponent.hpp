#pragma once

#include "engine/core/entities/Component.hpp"
#include "engine/core/entities/TransformComponent.hpp"
#include "engine/util/Vector3.hpp"

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
