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
        lastFrameTime = std::chrono::steady_clock::now().time_since_epoch().count();
        if (!moduleManager.initializeModules(moduleInput)) {
            return;
        }

        running = true;
        while (running) {
            std::time_t currentFrameTime = std::chrono::steady_clock::now().time_since_epoch().count();
            moduleInput.dt = static_cast<float>(currentFrameTime - lastFrameTime) / 1000000000.0f; // Convert nanoseconds to seconds
            lastFrameTime = currentFrameTime;
            if (!moduleManager.updateModules(moduleInput)) {
                running = false;
            }

            input::InputSystem::poll();

            std::cout << "Frame Time: " << moduleInput.dt << " seconds\n";
        }
    }

    void Engine::stop() {
        running = false;
    }

} // namespace parteeengine