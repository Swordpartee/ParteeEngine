#include "engine/core/Engine.hpp"

namespace parteeengine {

    Engine::Engine() : moduleManager(), entityManager(), moduleInput(entityManager) {}

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
        if (!moduleManager.initializeModules(moduleInput)) {
            return;
        }

        running = true;
        while (running) {
            moduleInput.dt = 0.016f; // Placeholder for delta time calculation
            if (!moduleManager.updateModules(moduleInput)) {
                running = false;
            }
        }
    }

    void Engine::stop() {
        running = false;
    }

} // namespace parteeengine