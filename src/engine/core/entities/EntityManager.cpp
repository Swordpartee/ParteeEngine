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
        }
        return {id, generations[id]};
    }

    void EntityManager::destroyEntity(const Entity entity) {
        // Remove from current archetype
        generations[entity.id]++;
        freeIds.push_back(entity.id);

        // Remove all components for this entity
        for (auto& [compId, compArray] : entityComponents) {
            compArray->removeEntity(entity);
        }
    }

    bool EntityManager::isValid(const Entity& entity) const {
        return entity.id < generations.size() && generations[entity.id] == entity.generation;
    }

} // namespace parteeengine