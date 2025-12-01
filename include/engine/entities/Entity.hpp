#pragma once

#include <vector>
#include <memory>
#include "engine/entities/components/Component.hpp"

namespace ParteeEngine {

    template <typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;

    class Entity {
    public:
        Entity() = default;
        ~Entity() = default;

        template <IsComponent T>
        T& addComponent();

        template <IsComponent T>
        T* getComponent();

    private:
        std::vector<std::unique_ptr<Component>> components;
        
    };

    template <IsComponent T>
    T& addComponent() {
        auto component = std::make_unique<T>();
        T& ref = *component;
        components.emplace_back(std::move(component));
        return ref;
    }

    template <IsComponent T>
    T* getComponent() {
        for (const auto& comp : components) {
            if (auto ptr = dynamic_cast<T*>(comp.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

} // namespace ParteeEngine