#pragma once

#include "engine/core/entities/EntityManager.hpp"
#include "engine/core/entities/Component.hpp"
#include "engine/util/Color.hpp"

#include <functional>

namespace parteeengine::rendering {

    struct QuadRenderCommand {
        Transform2d transform;
        Color color;
    };

    struct RenderQuadComponent : public ComponentCRTP<RenderQuadComponent> {
        parteeengine::Color color;

        RenderQuadComponent() = default;
        RenderQuadComponent(parteeengine::Color color) : color(color) {}

        static GatherFunction gatherer() {
            return std::function<void(RenderFrame&, const parteeengine::EntityManager&)>([](RenderFrame& frame, const parteeengine::EntityManager& entityManager) {
                auto entities = entityManager.getEntityComponentPairs<RenderQuadComponent>();
                for (auto& [entity, quad] : entities) {
                    auto transform = entityManager.getComponent<TransformComponent2d>(entity);

                    frame.emit(QuadRenderCommand{
                        .transform = transform->transform,
                        .color = quad.color
                    });
                }
            });
        }

        static RenderFunction<OpenGLRenderer, QuadRenderCommand> openGLHandler() {
            return std::function<void(const RenderCommandBucket<QuadRenderCommand>&, const RenderContext<OpenGLRenderer>&)>([](const RenderCommandBucket<QuadRenderCommand>& bucket, [[maybe_unused]]const RenderContext<OpenGLRenderer>& context) {
                for (const auto& command : bucket.commands) {
                    // Set up OpenGL state for rendering the quad
                    glPushMatrix();
                    glTranslatef(command.transform.position.x, command.transform.position.y, 0.f);
                    glRotatef(command.transform.rotation, 0.f, 0.f, 1.f);
                    glScalef(command.transform.scale.x, command.transform.scale.y, 1.f);

                    glColor4f(command.color.r, command.color.g, command.color.b, command.color.a);

                    // Render a unit quad centered at the origin
                    glBegin(GL_QUADS);
                    glVertex2f(-0.5f, -0.5f);
                    glVertex2f(0.5f, -0.5f);
                    glVertex2f(0.5f, 0.5f);
                    glVertex2f(-0.5f, 0.5f);
                    glEnd();

                    glPopMatrix();
                }
            });
        }
    };

}