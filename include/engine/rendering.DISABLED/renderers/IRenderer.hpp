#pragma once

#include "engine/rendering/core/RenderFrame.hpp"
#include "engine/rendering/core/RenderCommandBucket.hpp"

#include <unordered_map>
#include <typeindex>

namespace parteeengine::rendering {

    class IWindow;

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool initialize(IWindow& window) = 0;

        virtual bool render(RenderFrame& frame) = 0;

        template<typename CommandType>
        void registerRenderFunction(std::function<void(std::vector<CommandType>&)> func) {
           renderFunctions[typeid(CommandType)] = [func](VirtualRenderCommandBucket& bucket) {
                func(bucket.commands);
            };
        }

    protected:
        IRenderer() = default;

        std::unordered_map<std::type_index, std::function<void(VirtualRenderCommandBucket&)>> renderFunctions;
        
    };
    
} // namespace parteeengine::rendering