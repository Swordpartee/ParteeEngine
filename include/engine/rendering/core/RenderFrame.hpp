#pragma once

#include "engine/rendering/core/RenderCommandBucket.hpp"

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace parteeengine::rendering {

    struct RenderFrame {
        std::unordered_map<std::type_index, std::unique_ptr<IRenderCommandBucket>> buckets;

        template<typename CommandType>
        void emit(CommandType command) {
            auto typeIndex = std::type_index(typeid(CommandType));
            auto it = buckets.find(typeIndex);
            if (it == buckets.end()) {
                auto bucket = std::make_unique<RenderCommandBucket<CommandType>>();
                bucket->commands.push_back(command);
                buckets[typeIndex] = std::move(bucket);
            } else {
                auto bucket = static_cast<RenderCommandBucket<CommandType>*>(it->second.get());
                bucket->commands.push_back(command);
            }
        }
    };
}