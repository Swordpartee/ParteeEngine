#pragma once

#include "Component.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    class TransformComponent: public Component {
    friend class Entity;
    
    public:
        const Vector3& getPosition() const { return position; }
        void setPosition(const Vector3& pos) { position = pos; }

        const Vector3& getRotation() const { return rotation; }
        void setRotation(const Vector3& rot) { rotation = rot; }

    protected:
        using Component::Component;

    private:
        Vector3 position = {0, 0, 0};
        Vector3 rotation = {0, 0, 0};
    };

    template <>
    struct ComponentTraits<TransformComponent>
    {
        static constexpr bool unique = true;
        static constexpr ComponentCategory categories = ComponentCategory::Transform;
    };

} // namespace ParteeEngine