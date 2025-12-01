#pragma once

#include <vector>
#include <type_traits>
#include <memory>

#include "modules/Module.hpp"
#include "entities/Entity.hpp"
#include "Window.hpp"

namespace ParteeEngine {

    // Module concept
    template <typename T>
    concept IsModule = std::is_base_of_v<Module, T>;

    struct ModuleUpdateInputs {
        std::vector<Entity>& entities;
    };

    class Engine {
    public:
        Engine();

        template <IsModule T>
        T& addModule();

        Entity &createEntity();

        void run();   

    private:
        Window window;

        std::vector<std::unique_ptr<Module>> modules;
        std::vector<Entity> entities;

        void update();
    };

    template <IsModule T>
    T& Engine::addModule() {
        auto module = std::make_unique<T>();
        T &ref = *module;
        modules.emplace_back(std::move(module));
        return ref;
    }

} // namespace ParteeEngine