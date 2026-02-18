#include "engine/core/modules/ModuleManager.hpp"

namespace parteeengine {

    bool ModuleManager::initializeModules(const ModuleInput& inputs) {
        for (auto& [type, module] : modules) {
            if (!module->initialize(inputs)) {
                return false;
            }
        }
        return true;
    }

    bool ModuleManager::updateModules(const ModuleInput& inputs) {
        for (auto& [type, module] : modules) {
            if (!module->update(inputs)) {
                return false;
            }
        }
        return true;
    }

} // namespace parteeengine