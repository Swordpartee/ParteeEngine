#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

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

        template <typename T>
        const T *getComponent() const;

        template <typename T>
        T &ensureComponent();

    private:
        std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    };

    template <typename T>
    T& Entity::addComponent() {
        auto comp = std::make_unique<T>(*this);
        T &ref = *comp;

        components[std::type_index(typeid(T))] = std::move(comp);

        ref.ensureDependencies();
        ref.onAttach();

        return ref;
    }

    template <typename T>
    T* Entity::getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
        {
            return static_cast<T *>(it->second.get());
        }
        return nullptr;
    }

    template <typename T>
    const T *Entity::getComponent() const
    {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
        {
            return static_cast<const T *>(it->second.get());
        }
        return nullptr;
    }

    template <typename T>
    T& Entity::ensureComponent()
    {
        if (auto *existing = getComponent<T>())
            return *existing;
        return addComponent<T>();
    }

} // namespace ParteeEngine