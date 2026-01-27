#pragma once

#include <typeindex>
#include <typeinfo>
#include <functional>

namespace ParteeEngine {

    class ComponentID {
    public:
        template <typename T>
        static ComponentID of() {
            static const ComponentID id(std::type_index(typeid(T)));
            return id;
        }

        bool operator==(const ComponentID& other) const {
            return typeIndex == other.typeIndex;
        }

        bool operator!=(const ComponentID& other) const {
            return typeIndex != other.typeIndex;
        }

    private:
        std::type_index typeIndex;

        explicit ComponentID(std::type_index idx) : typeIndex(idx) {}

        friend struct std::hash<ComponentID>;
    };

}

namespace std {
    template <>
    struct hash<ParteeEngine::ComponentID> {
        size_t operator()(const ParteeEngine::ComponentID& id) const {
            return std::hash<std::type_index>()(id.typeIndex);
        }
    };
}
