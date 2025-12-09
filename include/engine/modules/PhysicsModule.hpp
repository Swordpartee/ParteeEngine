#pragma once

#include "Module.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    class Entity;

    struct CollisionInfo {
        bool isColliding = false;
        Vector3 normal;          // Collision normal
        float penetration = 0.0f; // Penetration depth
        Entity* entityA = nullptr;
        Entity* entityB = nullptr;
    };

    // Oriented Bounding Box structure
    struct OBB {
        Vector3 center;
        Vector3 halfExtents;
        Vector3 axes[3];  // Local X, Y, Z axes (orientation)
        float radius = 0.0f; // Bounding sphere radius for broadphase
    };

    class PhysicsModule : public Module {
    friend class Engine;
    public:
        ~PhysicsModule() override = default;

        void initialize(const ModuleInputs& inputs) override;

        void update(const ModuleUpdateInputs& inputs) override;

        // Physics settings
        void setGravity(const Vector3& g) { gravity = g; }
        Vector3 getGravity() const { return gravity; }

    private:
        using Module::Module;

        Vector3 gravity{0.0f, 9.81f, 0.0f};

        // Physics integration
        void calculatePhysics(Entity& entity, float dt);

        // Collision detection
        void checkCollisions(std::deque<Entity>& entities);
        // CollisionInfo checkOBBCollision(const OBB& a, const OBB& b, Entity* entityA, Entity* entityB);
        
        // Build OBB from entity components
        // OBB buildOBB(Entity& entity);

        // Get rotation matrix axes from rotation vector (Euler angles)
        // void getRotationAxes(const Vector3& rotation, Vector3 axes[3]);

        // Separating Axis Test
        // bool testSeparatingAxis(const OBB& a, const OBB& b, const Vector3& axis, float& penetration);

        // Collision response
        // void resolveCollision(const CollisionInfo& collision);
    };

    template <>
    struct ModuleTraits<PhysicsModule> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Physics;
    };

} // namespace ParteeEngine