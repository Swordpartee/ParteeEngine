#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "CollisionSystem.hpp"
#include "Vector3.hpp"
#include "components/RenderComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/ColliderComponent.hpp"
#include <chrono>
#include <ctime>

namespace ParteeEngine {

    Engine::Engine(int width, int height) : width(width), height(height) {
        window = new Window(width, height);

        window->setRenderCallback([&]() { update() });

        // // Clear the screen
        // renderer->clear();

        // // Update physics
        // for (Entity &e : entities)
        // {
        //     auto physicsComp = e.getComponent<PhysicsComponent>();
        //     if (physicsComp)
        //         physicsComp->updatePhysics(e, dt); // Assuming a fixed timestep of 16ms
        // }

        // // Update collisions
        // collisionSystem->updateCollisions(entities);

        // // Update and render entities
        // for (Entity& e : entities) {
        //     auto renderComp = e.getComponent<RenderComponent>();
        //     if (renderComp) renderComp->render(e, *renderer);
        // }

        // // Present the frame
        // renderer->present();
    }

    void Engine::start() {
        lastTime = std::clock();

        for (auto& module : modules)
        {
            module.init();
        }

        window->show();
    }

    Entity& Engine::createEntity() {
        int newID = static_cast<int>(entities.size());
        entities.emplace_back(newID);
        return entities.back();
    }
    
    Engine::~Engine() {
        
    }

    void Engine::update() {
        std::unique_ptr<ModuleInput> input = std::make_unique<ModuleInput>();

        input->dt = (std::clock() - lastTime) / (float)CLOCKS_PER_SEC;
        lastTime = std::clock();

        static int frameCount = 0;
        frameCount++;
        input->framcount = frameCount;
        
        for (auto& module : modules)
        {
            module.update(*input);
        }
    }
}