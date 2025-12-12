#include "engine/Engine.hpp"

namespace ParteeEngine {

    Engine::Engine() {}

    Entity& Engine::createEntity() {
        // entities.emplace_back(*this);
        Entity newEntity;
        entities.push_back(std::move(newEntity));
        return entities.back();
    }

    void Engine::run() {
        // Initialize all modules
        ModuleInputs initInputs{};
        for (auto& module : modules) {
            module->initialize(initInputs);
        }

        // Main loop
        lastFrameTime = std::chrono::steady_clock::now();
        bool running = true;
        while (running) {
            update();
        }
    }

    void Engine::update()
    {
        // Calculate actual delta time
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        float dt = delta.count();
        
        dt = dt * 5.0f; // Speed up for testing

        ModuleUpdateInputs inputs{entities, dt};
        for (auto& module : modules) {
            module->update(inputs);
        }
    }
}