#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"

namespace ParteeEngine {

    struct RenderData {
        struct Mesh {
            // Mesh data
            // TODO: Define mesh structure
        } mesh;

        struct Sprite {
            unsigned int textureID = 0;
            float width = 0.0f;
            float height = 0.0f;
        } sprite;
    };

    class RenderComponent: public Component {
    friend class RenderableFactory;

    public:
        void ensureDependencies() override {
            owner.ensureComponent<TransformComponent>();
        }

        const RenderData &getRenderData() const { return renderData; }
        void setRenderData(const RenderData &data) { renderData = data; }

    protected:
        using Component::Component;

    private:
        RenderData renderData;
    };

    template <>
    struct ComponentTraits<RenderComponent> {
        static constexpr bool unique = true;
        static constexpr ComponentCategory categories = ComponentCategory::Render;
    };

} // namespace ParteeEngine