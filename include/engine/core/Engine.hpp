#pragma once

#include "engine/core/modules/ModuleManager.hpp"
#include "engine/core/entities/EntityManager.hpp"
#include "engine/input/InputSystem.hpp"

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

        template<ComponentType T>
        T& addComponent(Entity entity);

        void run();
        void stop();

    private:
        bool running = false; // Engine running state

        ModuleInput moduleInput; // Input passed to modules each frame

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

    template<ComponentType T>
    T& Engine::addComponent(Entity entity) {
        return entityManager.addComponent<T>(entity);
    }
} // namespace parteeengine