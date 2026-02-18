#pragma once

#include <cstdint>

namespace parteeengine {

    using ComponentId = uint32_t;

    // Assigns a unique ID to each component type at runtime.
    class ComponentRegistry {
    public:
        template<typename T>
        static ComponentId getComponentID();

    private:
        inline static ComponentId nextID = 0;
    };

    template<typename T>
    ComponentId ComponentRegistry::getComponentID() {
        static const ComponentId id = nextID++;
        return id;
    }

} // namespace parteeengine