#include "engine/core/modules/ModuleManager.hpp"

namespace parteeengine {

    void ModuleManager::initializeModules(const ModuleInput& inputs) {
        for (auto& [type, module] : modules) {
            module->initialize(inputs);
        }
    }

    void ModuleManager::updateModules(const ModuleInput& inputs) {
        for (auto& [type, module] : modules) {
            module->update(inputs);
        }
    }

} // namespace parteeengine