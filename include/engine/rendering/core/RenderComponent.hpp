#pragma once

#include "engine/util/Color.hpp"
#include "engine/core/entities/Component.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct RenderComponent: public Component {
        RenderComponent() = default;
    };

    struct QuadRenderComponent: public RenderComponent {
        parteeengine::Color color{1.f, 1.f, 1.f, 1.f};

        QuadRenderComponent() = default;
        QuadRenderComponent(float r, float g, float b, float a = 1.f) : color(r, g, b, a) {}
        QuadRenderComponent(const parteeengine::Color& col) :
            color(col) {}
    };

} // namespace parteeengine::rendering