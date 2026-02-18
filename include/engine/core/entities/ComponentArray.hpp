#pragma once

#include "engine/core/entities/Entity.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>

namespace parteeengine {

    // Type-erased base class for component storage. Allows the EntityManager to
    // manage heterogeneous component arrays through a uniform interface.
    class VirtualComponentArray {
    public:
        virtual ~VirtualComponentArray() = default;

        virtual void registerEntity(Entity entity) = 0;

        // Removes entity's component. No-op if entity doesn't have this component type.
        virtual void removeEntity(Entity entity) = 0;
    };

    // Typed, packed component storage. Uses swap-and-pop removal for O(1) delete.
    template<typename T>
    class ComponentArray : public VirtualComponentArray {
    public:
        void registerEntity(Entity entity) override;

        void removeEntity(Entity entity) override;

        T& get(Entity entity);

        std::vector<T>& getComponents();
        const std::vector<T>& getComponents() const;
        
        std::vector<std::pair<Entity, T>> getEntityComponentPairs();

    private:
        std::vector<Entity> indexToEntity;                   // Index → Entity ID
        std::unordered_map<Entity, size_t> entityToIndex;    // Entity ID → index
        std::vector<T> components;                             // Component data (packed)
    };

    template<typename T>
    void ComponentArray<T>::registerEntity(Entity entity) {
        if (entityToIndex.contains(entity)) {
            throw std::runtime_error("Entity already has component");
        }
        size_t index = components.size();
        components.push_back(T());
        indexToEntity.push_back(entity);
        entityToIndex[entity] = index;
    }

    template<typename T>
    void ComponentArray<T>::removeEntity(Entity entity) {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return;

        size_t removedIndex = it->second;
        size_t lastIndex = components.size() - 1;

        if (removedIndex != lastIndex) {
            components[removedIndex] = std::move(components[lastIndex]);
            Entity movedEntity = indexToEntity[lastIndex];
            entityToIndex[movedEntity] = removedIndex;
            indexToEntity[removedIndex] = movedEntity;
        }

        components.pop_back();
        indexToEntity.pop_back();
        entityToIndex.erase(it);
    }
    template<typename T>
    T& ComponentArray<T>::get(Entity entity) {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) {
            throw std::runtime_error("Entity does not have component");
        }
        return components[it->second];
    }

    template<typename T>
    std::vector<T>& ComponentArray<T>::getComponents() { return components; }

    template<typename T>
    const std::vector<T>& ComponentArray<T>::getComponents() const { return components; }

    template<typename T>
    std::vector<std::pair<Entity, T>> ComponentArray<T>::getEntityComponentPairs() {
        std::vector<std::pair<Entity, T>> pairs;
        for (size_t i = 0; i < components.size(); ++i) {
            pairs.emplace_back(indexToEntity[i], components[i]);
        }
        return pairs;
    }

} // namespace parteeengine