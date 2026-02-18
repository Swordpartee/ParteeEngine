#pragma once

#include "engine/rendering/components/RenderComponent.hpp"
#include "engine/rendering/core/RenderCommand.hpp"
#include "engine/core/entities/TransformComponent.hpp"
#include "engine/core/entities/UpdateComponent.hpp"
#include "engine/core/Engine.hpp"

namespace ParteeEngine::Rendering {

    class RenderComponentFactory {
    public:
        static Entity createQuadRenderComponent(Engine& engine, const Quad& quad) {
            return engine.createEntity().with<RenderComponent<DrawQuadCommand>>([&](auto& comp) {
                comp.getCommand() = DrawQuadCommand(quad);
            }).with<TransformComponent>([](auto& comp) {
            }).with<UpdateComponent>([](auto& comp) {
                comp.setUpdateFunction([](float dt, Entity& owner) {
                    const auto& position = owner.getComponent<TransformComponent>()->getPosition();
                    owner.getComponent<RenderComponent<DrawQuadCommand>>()->getCommand().data.x = position.x;
                    owner.getComponent<RenderComponent<DrawQuadCommand>>()->getCommand().data.y = position.y;
                });
            });
        }
    };
} // namespace ParteeEngine::Rendering