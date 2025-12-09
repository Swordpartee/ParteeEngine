#pragma once

#include "engine/entities/Entity.hpp"
#include "engine/entities/components/RenderComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"

namespace ParteeEngine {

    class Engine;

    class RenderableFactory {
    public:
        static Entity& createSquare(Engine& engine);

    private:
        static Entity& create(Engine& engine);
    };

} // namespace ParteeEngine