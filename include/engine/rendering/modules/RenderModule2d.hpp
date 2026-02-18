#pragma once

#include <memory>

#include "engine/core/modules/Module.hpp"
#include "engine/rendering/window/IWindow.hpp"
#include "engine/rendering/renderers/IRenderer.hpp"
#include "engine/rendering/core/RenderFrame.hpp"
#include "engine/core/entities/Entity.hpp"
#include "engine/rendering/components/RenderComponent2d.hpp"
#include "engine/rendering/core/RenderCommand.hpp"

namespace ParteeEngine::Rendering {

    template<typename T>
    concept IsRenderer = std::is_base_of_v<IRenderer, T>;

    template<IsRenderer T>
    class RenderModule2d : public ParteeEngine::Module {
    public:
        RenderModule2d(int width = 800, int height = 600) : Module(), window(IWindow::createPlatformWindow()) {
            WindowConfig config{width, height, "ParteeEngine 2D Renderer"};
            window->create(config);
        }
        ~RenderModule2d() override = default;

        bool initialize(const ParteeEngine::ModuleInputs& inputs) override {
            renderer = std::make_unique<T>();
            if (!renderer->initialize(*window)) { return false; }
            return true;
        }

        bool update(const ParteeEngine::ModuleUpdateInputs& inputs) override {
            RenderFrame frame;
            for (const auto& entity : inputs.entities) {
                if (entity.hasComponent<RenderComponent2d<DrawQuadCommand>>()) {
                    const auto& comp = entity.getComponent<RenderComponent2d<DrawQuadCommand>>();
                    frame.commands.push_back(std::make_unique<DrawQuadCommand>(comp->getCommand()));
                }
                if (entity.hasComponent<RenderComponent2d<DrawSpriteCommand>>()) {
                    const auto& comp = entity.getComponent<RenderComponent2d<DrawSpriteCommand>>();
                    frame.commands.push_back(std::make_unique<DrawSpriteCommand>(comp->getCommand()));
                }
            }

            return renderer->render(frame) && window->swapBuffers() && window->pollEvents();
        }

    private:
        std::unique_ptr<IWindow> window;
        std::unique_ptr<T> renderer;
    };
}