#pragma once

#include "engine/rendering/renderers/IRenderer.hpp"
#include "engine/rendering/windows/IWindow.hpp"
#include "engine/rendering/core/RenderBinding.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <typeindex>

namespace parteeengine::rendering {

    template<typename CommandType>
    struct RenderCommandBucket;
    struct ICommandBucket;

    class OpenGLRenderer : public IRenderer {
    public:
        bool initialize(IWindow& window) override;
        bool render(RenderFrame& frame,  IWindow& window) override;

        template<typename TCommand>
        void registerHandler(RenderFunction<OpenGLRenderer, TCommand> fn);

    private:
        HDC hdc = nullptr;
        HGLRC hglrc = nullptr;

        std::unordered_map<std::type_index, std::function<void(IRenderCommandBucket&, const RenderContext<OpenGLRenderer>&)>> handlers;
    };

    template<typename TCommand>
    void OpenGLRenderer::registerHandler(RenderFunction<OpenGLRenderer, TCommand> fn) {
        handlers[std::type_index(typeid(TCommand))] = [fn](IRenderCommandBucket& bucket, const RenderContext<OpenGLRenderer>& ctx) {
            auto& typed = static_cast<RenderCommandBucket<TCommand>&>(bucket);
            fn(typed, ctx);
        };
    }

}