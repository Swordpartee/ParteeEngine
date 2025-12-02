#pragma once

#include "engine/Engine.hpp"
#include "engine/entities/Entity.hpp"
#include "engine/entities/components/RenderComponent.hpp"

namespace ParteeEngine {

    class RenderableFactory {
    public:
        static Entity& createSquare(Engine& engine);

    private:
        static Entity &create(Engine &engine);
    };

} // namespace ParteeEngine