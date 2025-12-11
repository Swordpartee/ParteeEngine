#pragma once

#include "Module.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    class Entity;

    struct OBB;

    struct CollisionManifold {
        bool colliding = false;
        Vector3 normal{};      // from A to B
        float penetration = 0; // overlap distance along normal
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

        Vector3 gravity{0.0f, 10.0f, 0.0f};

        // Physics integration
        void calculatePhysics(Entity& entity, float dt);

        // Collision detection
        void checkCollisions(std::deque<Entity>& entities);

        CollisionManifold computeOBBManifold(const OBB &a, const OBB &b) const;

        float projectExtent(const OBB &obb, const Vector3 &axis) const;
    
    };

    template <>
    struct ModuleTraits<PhysicsModule> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Physics;
    };

} // namespace ParteeEngine