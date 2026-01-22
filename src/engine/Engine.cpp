#include "engine/Engine.hpp"

#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"
#include "engine/input/devices/Mouse.hpp"

#include <memory>
#include <iostream>

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

        InputSystem::registerDevice(std::make_unique<Keyboard>());
        InputSystem::registerDevice(std::make_unique<Mouse>());

        // Main loop
        lastFrameTime = std::chrono::steady_clock::now();
        running = true;
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

        // Bind to the registered keyboard device; inputID uses Windows virtual-key codes ('D' here).
        if (InputSystem::isActive(Bindings::KeyEscape)) {
            running = false;
        }

        // std::cout << "Mouse Position: (" 
        //     << InputSystem::getAnalog(Bindings::MouseX) << ", " 
        //     << InputSystem::getAnalog(Bindings::MouseY) << ")" << std::endl;
    }
}