#pragma once

#include <vector>

#include "entity.hpp"

namespace ParteeEngine {
    class Window;
    class Renderer;
    class CollisionSystem; 

    class Engine {

        public:
            Engine(int width, int height);
            ~Engine();

            void start();

            Entity& createEntity();

        private:
            int width;
            int height;

            void update();

            Window* window;
            Renderer* renderer;
            CollisionSystem* collisionSystem;

            std::vector<Entity> entities;
    };
} // namespace ParteeEngine
