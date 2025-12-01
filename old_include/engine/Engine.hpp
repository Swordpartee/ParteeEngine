#pragma once

#include <vector>

#include "Entity.hpp"
#include "Module.hpp"

namespace ParteeEngine {
    class Window;
    class Renderer;
    class CollisionSystem; 

    struct ModuleInput {
        float dt;
        int framcount;
        std::vector<Entity>& entities;
    }

    struct ModuleOutput {}

    class Engine {

        public:
            Engine(int width, int height);
            ~Engine();

            void start();

            Entity& createEntity();

            template<typename Module>
            void addModule();

            template<typename Module>
            Module* getComponent();

        private:
            int width;
            int height;

            float lastTime;

            void update();

            Window* window;

            std::vector<Entity> entities;
            std::vector<Module> modules;
    };

    template <typename Module>
    void Engine::addModule()
    {
        modules.emplace_back(Module());
    }

    template <typename Module>
    Module* Engine::getComponent()
    {
        for (auto& module : modules)
        {
            Module* mod = dynamic_cast<Module*>(&module);
            if (mod)
                return mod;
        }
        return nullptr;
    }
} // namespace ParteeEngine
