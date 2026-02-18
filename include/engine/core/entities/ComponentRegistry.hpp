#pragma once

#include <cstdint>

namespace parteeengine {

    using ComponentId = uint32_t;

    class ComponentRegistry {
    public:
        template<typename T>
        static ComponentId getComponentID();

    private:
        inline static ComponentId nextID = 0;
    };

    template<typename T>
    static ComponentId ComponentRegistry::getComponentID() {
        static const ComponentId id = nextID++;
        return id;
    }

} // namespace parteeengine