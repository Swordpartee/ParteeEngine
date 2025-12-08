#include "engine/Engine.hpp"

namespace ParteeEngine {

    Engine::Engine() : window(800, 600) {}

    Entity& Engine::createEntity() {
        // entities.emplace_back(*this);
        Entity newEntity;
        entities.push_back(std::move(newEntity));
        return entities.back();
    }

    void Engine::run() {
        window.show();

        while (window.processMessages()) {
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
        // Clamp dt to avoid huge jumps (e.g., during debugging)
        if (dt > 0.1f)
            dt = 0.016f;

        ModuleUpdateInputs inputs{entities, dt};
        for (auto& module : modules) {
            module->update(inputs);
        }
    }
}