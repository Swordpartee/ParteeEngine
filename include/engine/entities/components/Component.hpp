#pragma once

#include "ComponentTraits.hpp"

namespace ParteeEngine {

    class Entity;

    class Component {
    friend class Entity;
    
    public:
        virtual ~Component() = default;

        virtual void onAttach() {};
        virtual void ensureDependencies() {};

    protected:
        Component(Entity& owner) : owner(owner) {}

        Entity& owner;
        
    };

    template <>
    struct ComponentTraits<Component> {
        static constexpr bool unique = false;
        static constexpr bool isVirtual = true;
        static constexpr ComponentCategory categories = ComponentCategory::None;
    };

} // namespace ParteeEngine