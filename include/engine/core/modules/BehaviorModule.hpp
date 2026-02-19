#pragma once

#include "engine/core/modules/Module.hpp"
#include "engine/core/entities/EntityManager.hpp"
#include "engine/core/entities/BehaviorComponent.hpp"

namespace parteeengine {

    struct ModuleInput;

    class BehaviorModule : public Module {
    public:
        ~BehaviorModule() override = default;    

        bool initialize(const ModuleInput& input) override;
        bool update(const ModuleInput& input) override;
    };

} // namespace parteeengine