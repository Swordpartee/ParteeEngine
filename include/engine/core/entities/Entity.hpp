#pragma once

namespace parteeengine {

    using EntityId = uint32_t;
    using Generation = uint32_t;

    struct Entity {
        EntityId id;
        Generation generation;

        bool operator==(const Entity& other) const {
            return id == other.id && generation == other.generation;
        }
    };
}