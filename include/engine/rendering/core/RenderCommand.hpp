#pragma once

#include "engine/rendering/core/RenderComponent.hpp"

namespace parteeengine::rendering {

    struct RenderCommand {
        virtual ~RenderCommand() = default;

        virtual std::type_index getType() const = 0;
    };

    struct QuadRenderCommand : public RenderCommand {
        RenderQuad quad;

        QuadRenderCommand() = default;
        QuadRenderCommand(const RenderQuad& quad) : quad(quad) {}

        std::type_index getType() const override {
            return typeid(QuadRenderCommand);
        }
    };

} // namespace parteeengine::rendering