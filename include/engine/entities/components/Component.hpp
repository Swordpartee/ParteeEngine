#pragma once

namespace ParteeEngine {

    class Entity;

    class Component {
    public:
        Component(Entity& owner) : owner(owner) {}
        virtual ~Component() = default;

        virtual void onAttach() {};
        virtual void ensureDependencies() {};

    protected:
        Entity& owner;
        
    };

} // namespace ParteeEngine