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
        void registerEntity(Entity entity) override {
            if (entityToIndex.contains(entity.id)) {
                throw std::runtime_error("Entity already has component");
            }
            size_t index = components.size();
            components.push_back(T());
            indexToEntity.push_back(entity.id);
            entityToIndex[entity.id] = index;
        }

        void removeEntity(Entity entity) override {
            auto it = entityToIndex.find(entity.id);
            if (it == entityToIndex.end()) return;

            size_t removedIndex = it->second;
            size_t lastIndex = components.size() - 1;

            if (removedIndex != lastIndex) {
                components[removedIndex] = std::move(components[lastIndex]);
                EntityId movedId = indexToEntity[lastIndex];
                entityToIndex[movedId] = removedIndex;
                indexToEntity[removedIndex] = movedId;
            }

            components.pop_back();
            indexToEntity.pop_back();
            entityToIndex.erase(it);
        }

        T& get(Entity entity) {
            auto it = entityToIndex.find(entity.id);
            if (it == entityToIndex.end()) {
                throw std::runtime_error("Entity does not have component");
            }
            return components[it->second];
        }

        std::vector<T>& getComponents() { return components; }
        const std::vector<T>& getComponents() const { return components; }

    private:
        std::vector<EntityId> indexToEntity;                   // Index → Entity ID
        std::unordered_map<EntityId, size_t> entityToIndex;    // Entity ID → index
        std::vector<T> components;                             // Component data (packed)
    };

} // namespace parteeengine