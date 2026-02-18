#pragma once

#include "engine/util/Color.hpp"
#include "engine/core/entities/Component.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct RenderComponent: public Component {
        virtual ~RenderComponent() = default;

        virtual std::type_index getType() const = 0;
    };

    struct Quad: public RenderComponent {
        float x = 0.f, y = 0.f;
        float width, height;
        parteeengine::Color color;

        std::type_index getType() const override {
            return typeid(Quad);
        }

        Quad() = default;
        Quad(float w, float h, const parteeengine::Color& col = {1.f, 1.f, 1.f, 1.f}) :
            width(w), height(h), color(col) {}
    };
    using QuadRenderComponent = Quad;


} // namespace parteeengine::rendering