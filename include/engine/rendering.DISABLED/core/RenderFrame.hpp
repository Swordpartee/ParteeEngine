#pragma once

#include "engine/rendering/core/RenderCommand.hpp"
#include "engine/rendering/core/RenderCommandBucket.hpp"

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace parteeengine::rendering {

    struct RenderFrame {
    public:
        template<IsRenderCommand CommandType>
        std::vector<CommandType>& createCommandBucket() {
            std::type_index type = typeid(CommandType);
            if (!commandBuckets.contains(type)) {
                commandBuckets[type] = std::make_unique<RenderCommandBucket<CommandType>>();
            }
            return static_cast<RenderCommandBucket<CommandType>&>(*commandBuckets[type]).commands;
        }

        template<IsRenderCommand CommandType>
        void clearCommandBucket() {
            std::type_index type = typeid(CommandType);
            if (commandBuckets.contains(type)) {
                static_cast<RenderCommandBucket<CommandType>&>(*commandBuckets[type]).commands.clear();
            }
        }

        void clearAllCommandBuckets() {
            for (auto& [type, bucket] : commandBuckets) {
                bucket->commands.clear();
            }
        }

        template<IsRenderCommand CommandType>
        std::vector<CommandType>* getCommandBucket() {
            std::type_index type = typeid(CommandType);
            if (commandBuckets.contains(type)) {
                return &static_cast<RenderCommandBucket<CommandType>&>(*commandBuckets[type]).commands;
            }
            return nullptr;
        }

        const std::unordered_map<TypeId, std::unique_ptr<VirtualRenderCommandBucket>>& getBuckets() const {
            return commandBuckets;
        }

        template<IsRenderCommand CommandType>
        bool hasCommandBucket() const {
            return commandBuckets.contains(typeid(CommandType));
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<VirtualRenderCommandBucket>> commandBuckets;
    };

} // namespace parteeengine::rendering