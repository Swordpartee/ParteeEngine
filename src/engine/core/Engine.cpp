#include "engine/core/Engine.hpp"

namespace parteeengine {

    Engine::Engine() : moduleManager(), entityManager(), moduleInput(entityManager), interpreter(this) {
        // Expose engine interface to the scripting environment
        interpreter.ExposeObject("Engine", getEngineInterface());
    }

    Engine::~Engine() {}

    interpreter::Value Engine::getEngineInterface() {
        interpreter::ObjectBuilder builder;
        builder.addFunction("createEntity", [this](std::vector<interpreter::Value> args) -> interpreter::Value {
            if (!args.empty()) {
                throw std::runtime_error("createEntity does not take any arguments");
            }
            Entity entity = createEntity();
            return interpreter::ObjectBuilder{}
                .addProperty("__id", interpreter::Value(static_cast<double>(entity.id)))
                .addProperty("__generation", interpreter::Value(static_cast<double>(entity.generation)))
                .build();
        });
        // Additional engine functions can be exposed here
        return builder.build();
    }

    void Engine::addScript(std::string script) {
        scripts.push_back(script);
    }

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

        for (const auto& script : scripts) {
            interpreter.interpret(script);
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