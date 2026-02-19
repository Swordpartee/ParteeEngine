#pragma once

#include "engine/rendering/core/RenderComponent.hpp"

#include <typeindex>

namespace parteeengine::rendering {

    struct VirtualRenderCommand {
        virtual ~VirtualRenderCommand() = default;

        virtual std::type_index getType() const = 0;
    };

    template<typename Derived>
    struct RenderCommandCRTP : public VirtualRenderCommand {
        std::type_index getType() const override {
            return typeid(Derived);
        }
    };

    struct QuadRenderCommand : public RenderCommandCRTP<QuadRenderCommand> {
        Transform2d quad;
        parteeengine::Color color;

        QuadRenderCommand() = default;
        QuadRenderCommand(const Transform2d& quad, const parteeengine::Color& color) : quad(quad), color(color) {}
    };

} // namespace parteeengine::rendering