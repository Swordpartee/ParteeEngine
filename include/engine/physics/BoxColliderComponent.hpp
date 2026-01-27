#pragma once

#include "ColliderComponent.hpp"

namespace ParteeEngine {
    
    // Get OBB (Oriented Bounding Box) with rotation
    struct OBB {
        ParteeEngine::Vector3 center;
        ParteeEngine::Vector3 halfExtents;
        ParteeEngine::Vector3 axes[3]; // Local X, Y, Z axes
    };

    class BoxColliderComponent : public ColliderComponent {
      public:
        float getBoundingSphereRadius() const override {
            auto transform = owner->getComponent<TransformComponent>();
            auto halfExtends = transform->getScale() * 0.5f;
            float radius = halfExtends.length();
            return radius;
        }

        OBB getOBB() const {
            auto transform = owner->getComponent<TransformComponent>();
            Vector3 position = transform->getPosition();
            Vector3 scale = transform->getScale();
            Vector3 rotation = transform->getRotation();

            OBB obb;
            obb.center = position;
            obb.halfExtents = scale * 0.5f;

            // Convert rotation (in degrees or radians) to axes
            // This assumes your rotation is Euler angles
            computeAxes(rotation, obb.axes);

            return obb;
        }

      private:
        // Convert Euler angles to rotation axes
        void computeAxes(const Vector3 &frontNormal, Vector3 axes[3]) const {
            // Check if rotation is zero or very small - use identity axes
            if (frontNormal.lengthSquared() < 0.0001f) {
                axes[0] = Vector3(1.0f, 0.0f, 0.0f);  // Right
                axes[1] = Vector3(0.0f, 1.0f, 0.0f);  // Up
                axes[2] = Vector3(0.0f, 0.0f, 1.0f);  // Forward
                return;
            }

            // Z-axis (forward vector) - the front normal
            axes[2] = frontNormal.normalized();

            // Choose a temporary up vector that's not parallel to the front
            // normal
            Vector3 tempUp = Vector3(0.0f, 1.0f, 0.0f);

            // If front normal is too close to up/down, use a different temp up
            if (std::abs(axes[2].dot(tempUp)) > 0.9f) {
                tempUp = Vector3(1.0f, 0.0f, 0.0f);
            }

            // X-axis (right vector) = cross(tempUp, forward)
            axes[0] = tempUp.cross(axes[2]).normalized();

            // Y-axis (up vector) = cross(forward, right)
            axes[1] = axes[2].cross(axes[0]).normalized();
        }
    };

} // namespace ParteeEngine