#include "engine/Engine.hpp"

namespace ParteeEngine {

    Engine::Engine() : window(800, 600) {}

    Entity& Engine::createEntity() {
        entities.emplace_back();
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
        ModuleUpdateInputs inputs{entities};
        for (auto& module : modules) {
            module->update(inputs);
        }
    }
}