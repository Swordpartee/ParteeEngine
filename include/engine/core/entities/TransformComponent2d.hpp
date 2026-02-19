#pragma once

#include "engine/core/entities/Component.hpp"
#include "engine/util/Vector2.hpp"

namespace parteeengine {

    struct Transform2d {
        Vector2 position{0.f, 0.f};
        float rotation = 0.f; // In degrees
        Vector2 scale{100.f, 100.f};

        Transform2d() = default;
        Transform2d(float x, float y) :
            position(x, y) {};
        Transform2d(const Vector2& pos, float rot, const Vector2& scl) :
            position(pos), rotation(rot), scale(scl) {}
    };

    struct TransformComponent2d: public Component {
        Transform2d transform;

        TransformComponent2d() = default;
        TransformComponent2d(float x, float y) :
            transform(x, y) {}
        TransformComponent2d(const Vector2& pos, float rot, const Vector2& scl) :
            transform(pos, rot, scl) {}

    };
}