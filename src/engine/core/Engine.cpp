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
        if (!moduleManager.initializeModules({entityManager})) {
            return;
        }

        running = true;
        while (running) {
            if (!moduleManager.updateModules({entityManager})) {
                running = false;
            }
        }
    }

    void Engine::stop() {
        running = false;
    }

} // namespace parteeengine