#pragma once

namespace ParteeEngine {

    class Entity;

    class Component {
    public:
        virtual ~Component() = default;

        virtual void requireDependencies() {}

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(float dt) {}

    protected:
        Entity* owner = nullptr;

    private:
        friend class Entity;
        void setOwner(Entity* entity) { owner = entity; }
    };

}
