#pragma once

#include <cstdint>

namespace parteeengine {

    using EntityId = uint32_t;
    using Generation = uint32_t;

    // Lightweight handle to an entity. Carries an ID and generation counter
    // so stale handles to destroyed entities can be detected.
    struct Entity {
        EntityId id;
        Generation generation;

        bool operator==(const Entity& other) const {
            return id == other.id && generation == other.generation;
        }
    };

} // namespace parteeengine