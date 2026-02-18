#pragma once

#include "engine/core/entities/ComponentRegistry.hpp"
#include "engine/core/entities/ComponentArray.hpp"
#include "engine/core/entities/Entity.hpp"

#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cstdint>

namespace parteeengine {

    class ArchetypeManager {
    public:
        ArchetypeManager() = default;

        // Add an entity with a component value
        template<typename T>
        void addComponent(Entity entity, const T& component);

        // Get a component for an entity
        template<typename T>
        T& getComponent(Entity entity);

        template<typename T>
        ComponentArray<T>* getComponentArray();

        // Remove an entity (swap-and-pop)
        void removeEntity(Entity entity);

    private:
        std::vector<Entity> entities;  // Entity IDs at each row
        std::unordered_map<EntityId, size_t> entityToRow;  // Entity ID → row index
        std::unordered_map<ComponentId, std::unique_ptr<VirtualComponentArray>> componentArrays;
    };

    template<typename T>
    void ArchetypeManager::addComponent(Entity entity, const T& component) {
        ensureComponentArray<T>();
        ComponentId id = ComponentRegistry::getComponentID<T>();
        auto* array = static_cast<ComponentArray<T>*>(componentArrays[id].get());

        // If entity already exists in this archetype, update
        auto it = entityToRow.find(entity);
        if (it != entityToRow.end()) {
            array->get(it->second) = component;
            return;
        }

        // New entity in this archetype
        size_t row = entities.size();
        entities.push_back(entity);
        entityToRow[entity.id] = row;
        array->qaddComponent(&component);
    }

    template<typename T>
    T& ArchetypeManager::getComponent(Entity entity) {
        auto it = entityToRow.find(entity.id);
        if (it == entityToRow.end()) {
            throw std::runtime_error("Entity not found in archetype");
        }
        ComponentId id = ComponentRegistry::getComponentID<T>();
        return static_cast<ComponentArray<T>*>(componentArrays[id].get())->get(it->second);
    }

    template<typename T>
    ComponentArray<T>* ArchetypeManager::getComponentArray() {
        ComponentId id = ComponentRegistry::getComponentID<T>();
        if (componentArrays.find(id) == componentArrays.end()) {
            componentArrays[id] = std::make_unique<ComponentArray<T>>();
        }
        return static_cast<ComponentArray<T>*>(componentArrays[id].get());
    }

} // namespace parteeengine