#pragma once

#include "modules/Module.hpp"
#include "engine/core/entities/Entity.hpp"

#include <deque>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <chrono>

namespace ParteeEngine {

    // Module concept
    template <typename T>
    concept IsModule = std::is_base_of_v<Module, T>;

    struct ModuleInputs;
    struct ModuleUpdateInputs;

    class Engine {
    public:
        Engine();

        template <IsModule T>
        void addModule();

        template <IsModule T, typename ConfigFunct>
        Engine& addModule(ConfigFunct&& configure) {
            auto& comp = addComponent<T>();
            configure(comp);
            return *this;
        };

        Entity& createEntity();

        void run();   

    private:
        bool running = false;
        
        std::vector<std::unique_ptr<Module>> modules;
        std::deque<Entity> entities;

        ModuleInputs moduleInputs{};

        std::chrono::steady_clock::time_point lastFrameTime;

        void update();
    };

    template <IsModule T>
    void Engine::addModule() {
        // Check if module type already exists
        if (std::find_if(modules.begin(), modules.end(), [](const auto &mod)
                         { return typeid(*mod) == typeid(T); }) != modules.end()) {
            throw std::runtime_error("Module type already present");
        }

        modules.emplace_back(std::make_unique<T>());
        modules.back()->initialize(moduleInputs);
    }

} // namespace ParteeEngine