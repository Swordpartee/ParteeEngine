#pragma once

#include "engine/util/Color.hpp"
#include "engine/core/entities/Component.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct RenderComponent: public Component {
        virtual ~RenderComponent() = default;
    };

    struct RenderQuad {
        Transform2d transform;
        parteeengine::Color color;
    };

    struct QuadRenderComponent: public RenderComponent {
        RenderQuad quad;

        QuadRenderComponent() = default;
        QuadRenderComponent(float w, float h, const parteeengine::Color& col = {1.f, 1.f, 1.f, 1.f}) :
            quad(Transform2d({0.f, 0.f}, 0.f, {w, h}), col) {}
    };

} // namespace parteeengine::rendering