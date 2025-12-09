#pragma once

#include "../Window.hpp"
#include "ModuleTraits.hpp"
#include <deque>

namespace ParteeEngine {

    class Entity;

    struct ModuleInputs {
        Window &window;
    };
    struct ModuleUpdateInputs {
        std::deque<Entity> &entities;
        float deltaTime;
    };

    class Module {
    public:
        virtual ~Module() = default;

        virtual void initialize(const ModuleInputs& inputs) = 0;

        virtual void update(const ModuleUpdateInputs& inputs) = 0;

    protected:
      Module() = default;
    };

} // namespace ParteeEngine