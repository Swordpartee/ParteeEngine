#pragma once

#include "engine/core/modules/Module.hpp"
#include "engine/core/entities/EntityManager.hpp"
#include "engine/core/entities/TestComponent.hpp"

#include <iostream>

namespace parteeengine {

    class TestModule : public Module {
    public:
        
        bool initialize(const ModuleInput& input) override {
            (void)input;
            std::cout << "TestModule initialized!" << std::endl;
            return true;
        }

        bool update(const ModuleInput& input) override {
            auto& comp = input.entityManager.getComponent<TestComponent>(Entity{0, 0});
            std::cout << comp.value << std::endl;
            return true;
        }
    };

} // namespace parteeengine