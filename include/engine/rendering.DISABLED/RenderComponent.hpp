#pragma once

#include "TransformComponent.hpp" 
#include "engine/entities/Entity.hpp"

#include <optional>
#include <stdexcept>

namespace ParteeEngine {

    struct RenderData {
        TransformComponent transform;
        struct Mesh {
            unsigned int vaoID = 0;
            unsigned int vertexCount = 0;
        } mesh;

        struct Sprite {
            unsigned int textureID = 0;
        } sprite;
    };

    class RenderComponent: public Component {
    public:
        const RenderData& getRenderData() const {
            if (!renderData.has_value()) {
                throw std::runtime_error("RenderData not set");
            }
            return *renderData;
        }

        void setRenderData(RenderData data) {
            renderData = std::move(data);
        }

        void requireDependencies() override {
            owner->ensureComponent<TransformComponent>();
        }

        void onAttach() override {
            auto* transform = owner->getComponent<TransformComponent>();
        }

    private:
        std::optional<RenderData> renderData;
    };

} // namespace ParteeEngine