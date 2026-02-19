#pragma once

#include "engine/core/modules/Module.hpp"
#include "engine/core/entities/EntityManager.hpp"
#include "engine/rendering/core/RenderComponent.hpp"
#include "engine/rendering/core/RenderFrame.hpp"
#include "engine/rendering/renderers/IRenderer.hpp"
#include "engine/rendering/window/IWindow.hpp"
#include "engine/util/Color.hpp"

#include <memory>

namespace parteeengine::rendering {

    // Renderer concept
    template<typename T>
    concept IsRenderer = std::derived_from<T, IRenderer>;
    
    class RenderModule2d: public Module {
    public:
        RenderModule2d();
        ~RenderModule2d() override = default;

        template<IsRenderer T>
        RenderModule2d& config(WindowConfig config);

        bool initialize(const ModuleInput& input) override;
        bool update(const ModuleInput& input) override;

    private:
        std::unique_ptr<IWindow> window;
        std::unique_ptr<IRenderer> renderer;
        RenderFrame frame;  // Reused each frame instead of allocating new one
    };

    template<IsRenderer T>
    RenderModule2d& RenderModule2d::config(WindowConfig config) {
        window->setConfig(config);
        renderer = std::make_unique<T>();
        return *this;
    }

} // namespace parteeengine::rendering