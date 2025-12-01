#pragma once

#include "Component.hpp"
#include "../../util/Vector3.hpp"

namespace ParteeEngine {

    class TransformComponent: public Component {
    public:
        TransformComponent(Entity& owner) : Component(owner), position(0,0,0), rotation(0,0,0) {}

        const Vector3& getPosition() const { return position; }
        void setPosition(const Vector3& pos) { position = pos; }

        const Vector3& getRotation() const { return rotation; }
        void setRotation(const Vector3& rot) { rotation = rot; }

    private:
        Vector3 position;
        Vector3 rotation;
    };

} // namespace ParteeEngine