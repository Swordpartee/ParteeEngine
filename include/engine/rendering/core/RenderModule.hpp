#pragma once

#include "engine/core/modules/Module.hpp"
#include "engine/rendering/windows/IWindow.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>

namespace parteeengine { class EntityManager; } // namespace parteeengine

namespace parteeengine::rendering {

    template<typename Renderer>
    struct RenderContext;
    template<typename CommandType>
    struct RenderCommandBucket;
    struct RenderFrame;
    
    using GatherFunction = std::function<void(RenderFrame&, const EntityManager&)>;
    template<typename Renderer, typename CommandType>
    using RenderFunction = std::function<void(RenderCommandBucket<CommandType>&, const RenderContext<Renderer>&)>;

    template<typename Renderer>
    class RenderModule : public Module {
    public:
        RenderModule() : window(IWindow::createPlatformWindow()), renderer() {};
        
        RenderModule<Renderer>& config(WindowConfig config);

        bool initialize(const ModuleInput& input);
        bool update(const ModuleInput& input);

        template <typename CommandType>
        RenderModule<Renderer>& registerComponent(GatherFunction gatherer, RenderFunction<Renderer, CommandType> renderFunc);
\
    private:
        std::unique_ptr<IWindow> window;
        Renderer renderer;

        std::vector<GatherFunction> gatherers;
        RenderFrame frame;
    };

    template<typename Renderer>
    RenderModule<Renderer>& RenderModule<Renderer>::config(WindowConfig config) {
        window->config(config);
    }

    template<typename Renderer>
    bool RenderModule<Renderer>::initialize([[maybe_unused]]const ModuleInput& input) {
        window->create();
        renderer.initialize(*window);
        return true;
    }

    template<typename Renderer>
    bool RenderModule<Renderer>::update(const ModuleInput& input) {
        frame.buckets.clear();
        for (auto gatherer : gatherers) {
            gatherer(frame, input.entityManager);
        }

        renderer.render(frame, *window);
        window->swapBuffers();

        return window->pollEvents();
    }

    template<typename Renderer>
    template<typename CommandType>
    RenderModule<Renderer>& RenderModule<Renderer>::registerComponent(GatherFunction gatherer, RenderFunction<Renderer, CommandType> renderFunc) {
        gatherers.emplace_back(gatherer);
        renderer.template registerHandler<CommandType>(renderFunc);
        return *this;
    }

} // namespace parteeengine::rendering