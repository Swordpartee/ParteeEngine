#pragma once

#include <cstdint>
#include <functional>

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

// Hash specialization for Entity to allow it to be used as a key in unordered_map
namespace std {
    template<>
    struct hash<parteeengine::Entity> {
        size_t operator()(const parteeengine::Entity& entity) const noexcept {
            // Combine hash of id and generation
            size_t h1 = std::hash<parteeengine::EntityId>()(entity.id);
            size_t h2 = std::hash<parteeengine::Generation>()(entity.generation);
            return h1 ^ (h2 << 1);
        }
    };
}
