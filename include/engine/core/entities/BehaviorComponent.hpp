#pragma once

#include "engine/core/entities/Component.hpp"

#include <functional>

namespace parteeengine {

    struct ModuleInput;

    struct BehaviorComponent : public ComponentCRTP<BehaviorComponent> {
        std::function<void(Entity entity, const ModuleInput&)> behavior;

        BehaviorComponent() = default;
        BehaviorComponent(std::function<void(Entity entity, const ModuleInput&)> behavior) : behavior(behavior) {}
    };

} // namespace parteeengine