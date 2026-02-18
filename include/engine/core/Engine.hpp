#pragma once

#include "engine/core/modules/ModuleManager.hpp"
#include "engine/core/entities/EntityManager.hpp"

namespace parteeengine {

    class Engine {
    public:
        Engine();
        ~Engine();

        // Adds a module of type T to the engine. Throws if a module of that type already exists.
        template<EngineModule T>
        T& createModule();

        // Gets a pointer to the module of type T, or nullptr if it doesn't exist.
        template<EngineModule T>
        T* getModule();

        // Entity management
        Entity createEntity();
        void destroyEntity(const Entity entity);
        // Checks if the given entity is valid (exists and has not been destroyed)
        bool isValidEntity(const Entity& entity) const;

        void run();

    private:
        bool running = false; // Engine running state

        EntityManager entityManager; // Manages entity creation and destruction
        ModuleManager moduleManager; // Manages engine modules
    };

    template<EngineModule T>
    T& Engine::createModule() {
        return moduleManager.createModule<T>();
    }

    template<EngineModule T>
    T* Engine::getModule() {
        return moduleManager.getModule<T>();
    }
} // namespace parteeengine