#pragma once

#include "engine/util/Color.hpp"
#include "engine/core/entities/Component.hpp"
#include "engine/util/Vector3.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct RenderComponent: public Component {
        virtual ~RenderComponent() = default;

        virtual std::type_index getType() const = 0;
    };

    struct QuadRenderComponent: public RenderComponent {
        Vector3 position{0.f, 0.f, 0.f};
        float width, height;
        parteeengine::Color color;

        std::type_index getType() const override {
            return typeid(QuadRenderComponent);
        }

        QuadRenderComponent() = default;
        QuadRenderComponent(float w, float h, const parteeengine::Color& col = {1.f, 1.f, 1.f, 1.f}) :
            width(w), height(h), color(col) {}
    };

} // namespace parteeengine::rendering