#pragma once

#include "engine/core/entities/Component.hpp"
#include "engine/core/entities/ComponentId.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace ParteeEngine {

    template <typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;

    class Entity {
    public:
        Entity() = default;
        ~Entity() = default;

        // Non-copyable, moveable
        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

        template <IsComponent T, typename ConfigFunct>
        Entity& with(ConfigFunct&& configure) {
            auto& comp = addComponent<T>();
            configure(comp);
            return *this;
        }

        // Get component - returns nullptr if not found
        template <IsComponent T> 
        T *getComponent() {

            // First try exact type match
            ComponentID id = ComponentID::of<T>();
            auto it = components.find(id);
            if (it != components.end()) {
                return static_cast<T *>(it->second.get());
            }

            // If exact match not found, search for a derived type
            for (auto &[key, component] : components) {
                T *casted = dynamic_cast<T *>(component.get());
                if (casted != nullptr) {
                    return casted;
                }
            }

            return nullptr;
        }

        // Get component const
        template <IsComponent T> 
        const T *getComponent() const {

            // First try exact type match
            ComponentID id = ComponentID::of<T>();
            auto it = components.find(id);
            if (it != components.end()) {
                return static_cast<const T *>(it->second.get());
            }

            // If exact match not found, search for a derived type
            for (auto &[key, component] : components) {
                const T *casted = dynamic_cast<const T *>(component.get());
                if (casted != nullptr) {
                    return casted;
                }
            }

            return nullptr;
        }

        template <IsComponent T> 
        void ensureComponent() {
            if (!hasComponent<T>()) {
                addComponent<T>();
            }
        }


        // Has component
        template <IsComponent T>
        bool hasComponent() const {
            ComponentID id = ComponentID::of<T>();
            return components.find(id) != components.end();
        }

        // Remove component
        template <IsComponent T>
        void removeComponent() {
            ComponentID id = ComponentID::of<T>();
            auto it = components.find(id);
            if (it != components.end()) {
                it->second->onDetach();
                components.erase(it);
            }
        }

        // Update all components
        void update(float dt) {
            for (auto& [id, component] : components) {
                component->onUpdate(dt);
            }
        }

    private:
        std::unordered_map<ComponentID, std::unique_ptr<Component>> components;

        friend class Component;

        // Add component - takes ownership
        template <IsComponent T>
        T& addComponent() {
            ComponentID id = ComponentID::of<T>();
            
            if (components.find(id) != components.end()) {
                throw std::runtime_error("Component already exists on entity");
            }

            auto component = std::make_unique<T>();
            T& ref = *component;
            
            component->setOwner(this);
            components[id] = std::move(component);

            ref.requireDependencies();
            
            ref.onAttach();
            
            return ref;
        }
    };

}
