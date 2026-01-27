#pragma once

#include "ModuleTraits.hpp"
#include <deque>

namespace ParteeEngine {

    class Entity;

    struct ModuleInputs {

    };
    struct ModuleUpdateInputs {
        std::deque<Entity> &entities;
        float deltaTime;
    };

    class Module {
    public:
        virtual ~Module() = default;

        virtual bool initialize(const ModuleInputs& inputs) = 0;

        virtual bool update(const ModuleUpdateInputs& inputs) = 0;

    protected:
      Module() = default;
    };

} // namespace ParteeEngine