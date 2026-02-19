#include "engine/core/modules/BehaviorModule.hpp"

namespace parteeengine {

    bool BehaviorModule::initialize([[maybe_unused]]const ModuleInput& input) {
        return true;
    };

    bool BehaviorModule::update(const ModuleInput& input) {
        for (auto& [entity, behaviorComponent] : input.entityManager.getEntityComponentPairs<BehaviorComponent>()) {
            behaviorComponent.behavior(entity, input);
        }
        return true;
    };

} // namespace parteeengine