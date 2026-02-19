#pragma once

#include "engine/core/entities/ComponentArray.hpp"
#include "engine/core/entities/ComponentRegistry.hpp"
#include "engine/core/entities/Entity.hpp"
#include "engine/core/entities/Component.hpp"

#include <vector>

namespace parteeengine {

    // Component concept
    template<typename T>
    concept ComponentType = std::is_base_of_v<Component, T>;

    class EntityManager {
    public:
        Entity createEntity();
        void destroyEntity(const Entity entity);
        bool isValid(const Entity& entity) const;

        template<ComponentType T>
        T& addComponent(Entity entity);

        template<ComponentType T>
        T* getComponent(Entity entity);

        template<ComponentType T>
        bool hasComponent(Entity entity);

        template<ComponentType T>
        std::vector<T>& getComponentArray();

        template<ComponentType T>
        std::vector<std::pair<Entity, T&>> getEntityComponentPairs();


    private:
        std::vector<Generation> generations;  // Generation count for each entity ID
        std::vector<EntityId> freeIds;  // Reusable entity IDs
        EntityId nextId = 0;  // Next entity ID to use if no free IDs

        std::unordered_map<ComponentId, std::unique_ptr<VirtualComponentArray>> entityComponents;  // Component ID → packed component array
    };

    template<ComponentType T>
    T& EntityManager::addComponent(Entity entity) {
        if (!isValid(entity)) {
            throw std::runtime_error("Invalid entity");
        }
        if (hasComponent<T>(entity)) {
            throw std::runtime_error("Entity already has component");
        }
        ComponentId id = ComponentRegistry::getComponentID<T>();
        if (entityComponents.find(id) == entityComponents.end()) {
            entityComponents[id] = std::make_unique<ComponentArray<T>>();
        }
        auto* array = static_cast<ComponentArray<T>*>(entityComponents[id].get());
        array->registerEntity(entity);
        return array->get(entity);
    }

    template<ComponentType T>
    T* EntityManager::getComponent(Entity entity) {
        if (!isValid(entity)) {
            throw std::runtime_error("Invalid entity");
        }
        ComponentId id = ComponentRegistry::getComponentID<T>();
        auto it = entityComponents.find(id);
        if (it == entityComponents.end()) {
            return nullptr; // Return nullptr if entity doesn't have this component
        }
        auto* array = static_cast<ComponentArray<T>*>(it->second.get());
        return &array->get(entity);
    }

    template<ComponentType T>
    bool EntityManager::hasComponent(Entity entity) {
        if (!isValid(entity)) {
            throw std::runtime_error("Invalid entity");
        }
        ComponentId id = ComponentRegistry::getComponentID<T>();
        auto it = entityComponents.find(id);
        if (it == entityComponents.end()) {
            return false;
        }
        auto* array = static_cast<ComponentArray<T>*>(it->second.get());
        try {
            array->get(entity);
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }   

    template<ComponentType T>
    std::vector<T>& EntityManager::getComponentArray() {
        ComponentId id = ComponentRegistry::getComponentID<T>();
        auto it = entityComponents.find(id);
        if (it == entityComponents.end()) {
            // throw std::runtime_error("No entities have this component");
            static std::vector<T> emptyVector;
            return emptyVector; // Return empty vector if no entities have this component
        }
        return static_cast<ComponentArray<T>*>(it->second.get())->getComponents();
    }

    template<ComponentType T>
    std::vector<std::pair<Entity, T&>> EntityManager::getEntityComponentPairs() {
        ComponentId id = ComponentRegistry::getComponentID<T>();
        auto it = entityComponents.find(id);
        if (it == entityComponents.end()) {
            // throw std::runtime_error("No entities have this component");
            static std::vector<std::pair<Entity, T&>> emptyVector;
            return emptyVector; // Return empty vector if no entities have this component
        }
        return static_cast<ComponentArray<T>*>(it->second.get())->getEntityComponentPairs();
    }

} // namespace parteeengine