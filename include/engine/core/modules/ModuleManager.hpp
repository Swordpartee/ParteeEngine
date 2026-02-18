#pragma once

#include "engine/core/modules/Module.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <stdexcept>

namespace parteeengine {

    // Module concept
    template<typename T>
    concept EngineModule = std::is_base_of_v<Module, T>;

    class ModuleManager {
    public:
        template<EngineModule T>
        T& createModule();

        template<EngineModule T>
        T* getModule();

        bool initializeModules(const ModuleInput& inputs);
        bool updateModules(const ModuleInput& inputs);

    private:
        std::unordered_map<std::type_index, std::unique_ptr<Module>> modules; // Map of module type to module instance
    };

    template<EngineModule T>
    T& ModuleManager::createModule() {
        if (modules.find(typeid(T)) != modules.end()) {
            throw std::runtime_error("Module of this type already exists");
        }
        modules.emplace(typeid(T), std::make_unique<T>());
        return *static_cast<T*>(modules[typeid(T)].get());
    }

    template<EngineModule T>
    T* ModuleManager::getModule() {
        auto it = modules.find(typeid(T));
        if (it != modules.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

} // namespace parteeengine