#pragma once

#include "engine/util/Color.hpp"

#include <typeindex>

namespace ParteeEngine::Rendering {
    struct RenderCommand {
        virtual ~RenderCommand() = default;
        virtual std::type_index type() const = 0;
        virtual std::type_index dataType() const = 0;

    protected:
        RenderCommand() = default;
    };

    template<typename DataType>
    struct RenderCommandCRTP: public RenderCommand {
        RenderCommandCRTP(): data() {}
        RenderCommandCRTP(const DataType& d): data(d) {}

        std::type_index type() const override {
            return typeid(RenderCommandCRTP<DataType>);
        }

        std::type_index dataType() const override {
            return typeid(DataType);
        }

        DataType data;
    };

    struct Quad {
        float x, y;
        float width, height;
        ParteeEngine::Color color;
    };
    using DrawQuadCommand = RenderCommandCRTP<Quad>;

    struct Sprite {
        float x, y;
        float width, height;
        float u1, v1, u2, v2; // Texture coordinates
        ParteeEngine::Color color;
    };
    using DrawSpriteCommand = RenderCommandCRTP<Sprite>;

    
}