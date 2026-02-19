#pragma once

#include "engine/rendering/core/RenderComponent.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct RenderCommand {
        virtual ~RenderCommand() = default;

        virtual std::type_index getType() const = 0;
    };

    struct QuadRenderCommand : public RenderCommand {
        Transform2d quad;
        parteeengine::Color color;

        QuadRenderCommand() = default;
        QuadRenderCommand(const Transform2d& quad, const parteeengine::Color& color) : quad(quad), color(color) {}

        std::type_index getType() const override {
            return typeid(QuadRenderCommand);
        }
    };

} // namespace parteeengine::rendering