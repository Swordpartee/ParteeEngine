#pragma once

#include <vector>
#include <memory>
#include "engine/entities/components/Component.hpp"

namespace ParteeEngine {

    // template <typename T>
    // concept IsComponent = std::is_base_of_v<Component, T>;

    class Entity {
    public:
        Entity() = default;
        ~Entity() = default;

        // Delete copy and allow move assignments
        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;
        Entity(Entity &&) noexcept = default;
        Entity &operator=(Entity &&) noexcept = default;

        template <typename T>
        T &addComponent();

        template <typename T>
        T *getComponent();

    private:
        std::vector<std::unique_ptr<Component>> components;
        
    };

    template <typename T>
    T& Entity::addComponent() {
        components.emplace_back(std::make_unique<T>());
        return *static_cast<T*>(components.back().get());
    }

    template <typename T>
    T* Entity::getComponent() {
        for (const auto& comp : components) {
            if (auto ptr = dynamic_cast<T*>(comp.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

} // namespace ParteeEngine