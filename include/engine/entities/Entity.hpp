#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <concepts>
#include <stdexcept>

#include "engine/entities/components/Component.hpp"
#include "engine/entities/components/ComponentTraits.hpp"

namespace ParteeEngine {

    template <typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;

    class Entity {
    public:
        Entity() = default;
        ~Entity() = default;

        // Delete copy and allow move assignments
        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;
        Entity(Entity &&) noexcept = default;
        Entity &operator=(Entity &&) noexcept = default;

        template <IsComponent T>
        T &addComponent();

        template <IsComponent T>
        T &addComponent(std::unique_ptr<T> component);

        template <IsComponent T>
        T *getComponent();

        template <IsComponent T>
        const T *getComponent() const;

        // Component* getComponent(ComponentCategory category);

        // const Component* getComponent(ComponentCategory category) const;

        template <IsComponent T>
        T &ensureComponent();

    private:
        std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
        unsigned componentTraitMask = 0;

    };

    template <IsComponent T>
    T &Entity::addComponent() {
        // Enforce unique per category
        const unsigned traitMask = static_cast<unsigned>(ComponentTraits<T>::categories);

        if (ComponentTraits<T>::unique && traitMask) {
            if ((componentTraitMask & traitMask) != 0) {
                throw std::runtime_error("Component category already present on entity");
            }
        }

        if (components.find(std::type_index(typeid(T))) != components.end()) {
            throw std::runtime_error("Component type already present on entity");
        }

        // Use Entity as intermediary to construct component
        auto component = std::unique_ptr<T>(new T(*this));
        T &ref = *component;
        componentTraitMask |= traitMask;
        components[typeid(T)] = std::move(component);
        ref.ensureDependencies();

        return ref;
    }

    template <IsComponent T>
    T &Entity::addComponent(std::unique_ptr<T> component) {
        if (!component)
        {
            throw std::runtime_error("Null component");
        }

        const unsigned traitMask = static_cast<unsigned>(ComponentTraits<T>::categories);

        if (ComponentTraits<T>::unique && traitMask)
        {
            if ((componentTraitMask & traitMask) != 0)
            {
                throw std::runtime_error("Component category already present on entity");
            }
        }
        if (components.find(std::type_index(typeid(T))) != components.end())
        {
            throw std::runtime_error("Component type already present on entity");
        }

        // If Component needs to know its Entity and was constructed without it,
        // you need a method like component->setEntity(*this); (add in Component base).
        T &ref = *component;
        components[typeid(T)] = std::move(component);
        componentTraitMask |= traitMask;
        ref.ensureDependencies();
        return ref;
    }

    template <IsComponent T>
    T* Entity::getComponent() {        
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T *>(it->second.get());
        }
        return nullptr;
    }

    template <IsComponent T>
    const T *Entity::getComponent() const {        
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<const T *>(it->second.get());
        }
        return nullptr;
    }

    // const Component* Entity::getComponent(ComponentCategory category) const {
    //     for (const auto& [typeIdx, compPtr] : components) {
    //         const auto& traits = ComponentTraits<std::remove_pointer_t<decltype(compPtr.get())>>;
    //         if ((static_cast<unsigned>(traits.categories) & static_cast<unsigned>(category)) != 0) {
    //             return compPtr.get();
    //         }
    //     }
    //     return nullptr;
    // }

    // Component* Entity::getComponent(ComponentCategory category) {
    //     for (const auto& [typeIdx, compPtr] : components) {
    //         const auto& traits = ComponentTraits<std::remove_pointer_t<decltype(compPtr.get())>>;
    //         if ((static_cast<unsigned>(traits.categories) & static_cast<unsigned>(category)) != 0) {
    //             return compPtr.get();
    //         }
    //     }
    //     return nullptr;
    // }

    template <IsComponent T>
    T& Entity::ensureComponent() {        
        if (auto *existing = getComponent<T>())
            return *existing;
        return addComponent<T>();
    }

} // namespace ParteeEngine