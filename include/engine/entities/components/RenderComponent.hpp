#pragma once

#include <stdexcept>
#include <optional>


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

        const RenderData &getRenderData() const {
            if (!renderData.has_value()) {
                throw std::runtime_error("RenderData not set");
            }
            return *renderData;
        };

    protected:
        using Component::Component;

    private:
        std::optional<RenderData> renderData;

        void setRenderData(RenderData data) {
            renderData = std::move(data);
        }
    };

    template <>
    struct ComponentTraits<RenderComponent> {
        static constexpr bool unique = true;
        static constexpr ComponentCategory categories = ComponentCategory::Render;
    };

} // namespace ParteeEngine