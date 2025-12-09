#pragma once

#include "engine/entities/components/Component.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    class TransformComponent: public Component {
    public:
        const Vector3& getPosition() const { return position; }
        void setPosition(const Vector3& pos) { position = pos; }

        const Vector3& getRotation() const { return rotation; }
        void setRotation(const Vector3& rot) { rotation = rot; }

        const Vector3& getScale() const { return scale; }
        void setScale(const Vector3& scl) { scale = scl; }

    private:
        Vector3 position = {0.0f, 0.0f, 0.0f};
        Vector3 rotation = {0.0f, 0.0f, 1.0f};
        Vector3 scale = {100.0f, 100.0f, 100.0f};
    };

} // namespace ParteeEngine