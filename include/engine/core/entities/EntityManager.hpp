#pragma once

#include "engine/core/entities/ArchetypeManager.hpp"
#include "engine/core/entities/ComponentRegistry.hpp"
#include "engine/core/entities/Entity.hpp"

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace parteeengine {

    using Archetype = uint64_t;

    class EntityManager {
    public:
        Entity createEntity();
        void destroyEntity(const Entity entity);
        bool isValid(const Entity& entity) const;

        template<typename T>
        void addComponent(Entity entity);

        template<typename T>
        T& getComponent(Entity entity);

    private:
        std::vector<EntityId> freeIds;
        std::vector<Generation> generations;
        EntityId nextId = 0;

        std::vector<Archetype> entityArchetypes;  // Entity ID → archetype bitmask
        std::unordered_map<Archetype, ArchetypeManager> archetypeData;  // Archetype mask → storage
    };

    template<typename T>
    void EntityManager::addComponent(Entity entity) {
        if (!isValid(entity)) {
            throw std::runtime_error("Invalid entity");
        }

        Archetype& archetype = entityArchetypes[entity.id];
        Archetype newArchetype = archetype | (1ULL << ComponentRegistry::getComponentID<T>());

        // If archetype is changing, move entity to new archetype storage
        if (newArchetype != archetype) {
            ArchetypeManager& oldStorage = archetypeData[archetype];
            ArchetypeManager& newStorage = archetypeData[newArchetype];

            if (archetype != 0) {
                oldStorage.removeEntity(entity);
            }
            newStorage.addComponent<T>(entity, T{}); // Add default component value

            archetype = newArchetype;
        }
    }

    template<typename T>
    T& EntityManager::getComponent(Entity entity) {
        if (!isValid(entity)) {
            throw std::runtime_error("Invalid entity");
        }

        Archetype archetype = entityArchetypes[entity.id];
        Archetype compMask = (1ULL << ComponentRegistry::getComponentID<T>());
        if ((archetype & compMask) == 0) {
            throw std::runtime_error("Entity does not have required component");
        }

        return archetypeData[archetype].getComponent<T>(entity.id);
    }

} // namespace parteeengine