#pragma once

#include "../Window.hpp"
#include "../entities/Entity.hpp"
#include "ModuleTraits.hpp"
#include <vector>

namespace ParteeEngine {

    struct ModuleInputs {
        Window &window;
    };
    struct ModuleUpdateInputs {
        std::vector<Entity> &entities;
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