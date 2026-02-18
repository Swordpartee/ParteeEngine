#include "engine/core/entities/EntityManager.hpp"

namespace parteeengine {

    Entity EntityManager::createEntity() {
        uint32_t id;
        if (!freeIds.empty()) {
            id = freeIds.back();
            freeIds.pop_back();
        } else {
            id = nextId++;
            generations.push_back(0);
            entityArchetypes.push_back(0); // No components yet
        }
        return {id, generations[id]};
    }

    void EntityManager::destroyEntity(const Entity entity) {
        // Remove from current archetype
        uint64_t archetype = entityArchetypes[entity.id];
        if (archetypeData.find(archetype) != archetypeData.end()) {
            archetypeData[archetype].removeEntity(entity);
        }

        entityArchetypes[entity.id] = 0;
        generations[entity.id]++;
        freeIds.push_back(entity.id);
    }

    bool EntityManager::isValid(const Entity& entity) const {
        return entity.id < generations.size() && generations[entity.id] == entity.generation;
    }

} // namespace parteeengine