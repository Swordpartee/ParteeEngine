#pragma once

#include <vector>
#include <type_traits>
#include <memory>

#include "modules/Module.hpp"
#include "entities/Entity.hpp"
#include "Window.hpp"

namespace ParteeEngine {

    // Module concept
    // template <typename T>
    // concept IsModule = std::is_base_of_v<Module, T>;

    struct ModuleInputs;
    struct ModuleUpdateInputs;

    class Engine {
    public:
        Engine();

        template <typename T>
        T& addModule();

        Entity &createEntity();

        void run();   

    private:
        Window window;

        std::vector<std::unique_ptr<Module>> modules;
        std::vector<Entity> entities;

        unsigned moduleCategoryMask = 0;

        ModuleInputs moduleInputs{window};

        void update();
    };

    template <typename T>
    T &Engine::addModule() {
        const unsigned catMask = static_cast<unsigned>(ModuleTraits<T>::categories);

        if (ModuleTraits<T>::unique && catMask) {
            if ((moduleCategoryMask & catMask) != 0u) {
                throw std::runtime_error("Module category already present");
            }
        }

        if (modules.find(std::type_index(typeid(T))) != modules.end()) {
            throw std::runtime_error("Module type already present");
        }

        modules.emplace_back(std::make_unique<T>());
        modules.back()->initialize(moduleInputs);
        return *static_cast<T *>(modules.back().get());
    }

} // namespace ParteeEngine