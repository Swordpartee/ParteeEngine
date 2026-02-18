#include "engine/core/entities/ArchetypeManager.hpp"

namespace parteeengine {

   void ArchetypeManager::removeEntity(Entity entity) {
        auto it = entityToRow.find(entity.id);
        if (it == entityToRow.end()) return;

        size_t row = it->second;
        size_t lastRow = entities.size() - 1;

        if (row != lastRow) {
            // Swap with last entity
            Entity lastEntity = entities[lastRow];
            entities[row] = lastEntity;
            entityToRow[lastEntity.id] = row;

            // Swap component data
            for (auto& [compId, array] : componentArrays) {
                array->removeComponent(row); // swap-and-pop
            }
        } else {
            for (auto& [compId, array] : componentArrays) {
                array->removeComponent(row);
            }
        }

        entities.pop_back();
        entityToRow.erase(entity.id);
    }

} // namespace parteeengine