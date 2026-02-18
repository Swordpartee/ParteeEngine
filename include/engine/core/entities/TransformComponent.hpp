#pragma once

#include "engine/core/entities/Component.hpp"
#include "engine/util/Vector3.hpp"

namespace ParteeEngine {

    class TransformComponent: public Component {
    public:
        Vector3& getPosition() { return position; }
        const Vector3& getPosition() const { return position; }
        void setPosition(const Vector3& pos) { position = pos; }

        Vector3& getRotation() { return rotation; }
        const Vector3& getRotation() const { return rotation; }
        void setRotation(const Vector3& rot) { rotation = rot; }

        Vector3& getScale() { return scale; }
        const Vector3& getScale() const { return scale; }
        void setScale(const Vector3& scl) { scale = scl; }

    private:
        Vector3 position = {0.0f, 0.0f, 0.0f};
        Vector3 rotation = {0.0f, 0.0f, 1.0f};
        Vector3 scale = {100.0f, 100.0f, 100.0f};
    };

} // namespace ParteeEngine