#include "engine/core/Engine.hpp"

namespace parteeengine {

    Engine::Engine() : entityManager() {}

    Engine::~Engine() {}

    Entity Engine::createEntity() {
        return entityManager.createEntity();
    }

    void Engine::destroyEntity(const Entity entity) {
        entityManager.destroyEntity(entity);
    }

    bool Engine::isValidEntity(const Entity& entity) const {
        return entityManager.isValid(entity);
    }

    void Engine::run() {
        // Initialize all modules
        moduleManager.initializeModules({});

        // Main loop
        running = true;
        while (running) {
            // Update all modules
            moduleManager.updateModules({});
        }
    }

} // namespace parteeengine