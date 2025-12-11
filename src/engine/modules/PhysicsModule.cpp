#include "engine/modules/PhysicsModule.hpp"

#include "engine/entities/Entity.hpp"
#include "engine/entities/components/ColliderComponent.hpp"
#include "engine/entities/components/BoxColliderComponent.hpp"
#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/math/Vector3.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <deque>

namespace ParteeEngine {

    void PhysicsModule::initialize(const ModuleInputs& inputs) {
        // Initialize physics settings
    }

    void PhysicsModule::update(const ModuleUpdateInputs& inputs) {
        // Calculate physics for all entities with rigidbodies
        for (auto& entity : inputs.entities) {
            if (entity.getComponent<RigidBodyComponent>()) {
                calculatePhysics(entity, inputs.deltaTime);
            }
        }

        // Check for collisions
        checkCollisions(inputs.entities);
    }

    void PhysicsModule::calculatePhysics(Entity& entity, float dt) {
        auto* rigidbody = entity.getComponent<RigidBodyComponent>();
        auto* transform = entity.getComponent<TransformComponent>();

        // Apply gravity
        Vector3 velocity = rigidbody->getVelocity();
        velocity += gravity * dt;
        
        // Apply friction (reduces velocity over time)
        // float frictionCoefficient = 0.99f;  // 0-1: higher = less friction
        // velocity *= frictionCoefficient;
        
        rigidbody->setVelocity(velocity);

        // Update position based on velocity
        Vector3 position = transform->getPosition();
        position += velocity * dt;
        transform->setPosition(position);
    }

    void PhysicsModule::checkCollisions(std::deque<Entity>& entities) {
        // Broadphase collision detection using bounding spheres
        std::vector<std::pair<Entity*, Entity*>> potentialCollisions;

        for (size_t i = 0; i < entities.size(); ++i) {
            auto* colliderA = entities[i].getComponent<ColliderComponent>();
            if (!colliderA) continue;

            for (size_t j = i + 1; j < entities.size(); ++j) {
                auto* colliderB = entities[j].getComponent<ColliderComponent>();
                if (!colliderB) continue;

                // Broadphase: Simple distance check between entity positions
                auto* transformA = entities[i].getComponent<TransformComponent>();
                auto* transformB = entities[j].getComponent<TransformComponent>();
                if (!transformA || !transformB) continue;

                Vector3 posA = transformA->getPosition();
                Vector3 posB = transformB->getPosition();

                float distSq = (posA - posB).lengthSquared();

                float radiusSum = colliderA->getBoundingSphereRadius() + colliderB->getBoundingSphereRadius();
                if (distSq <= radiusSum * radiusSum) {
                    potentialCollisions.emplace_back(&entities[i], &entities[j]);
                }
            }
        }

        // Narrowphase + resolution
        for (auto &pair : potentialCollisions) {
            auto *boxA = dynamic_cast<BoxColliderComponent *>(pair.first->getComponent<ColliderComponent>());
            auto *boxB = dynamic_cast<BoxColliderComponent *>(pair.second->getComponent<ColliderComponent>());
            
            if (!boxA || !boxB) continue;

            OBB obbA = boxA->getOBB();
            OBB obbB = boxB->getOBB();

            CollisionManifold m = computeOBBManifold(obbA, obbB);
            if (!m.colliding)
                continue;
            
            // Positional correction (only for dynamic objects with RigidBody)
            auto *rbA = pair.first->getComponent<RigidBodyComponent>();
            auto *rbB = pair.second->getComponent<RigidBodyComponent>();
            
            if (rbA || rbB) {
                const float kSlop = 0.001f;
                Vector3 correction = m.normal * (m.penetration - kSlop);
                
                auto *trA = pair.first->getComponent<TransformComponent>();
                auto *trB = pair.second->getComponent<TransformComponent>();
                
                // Apply correction proportionally based on which objects have physics
                if (rbA && rbB) {
                    // Both dynamic: split correction 50/50
                    trA->setPosition(trA->getPosition() - correction * 0.5f);
                    trB->setPosition(trB->getPosition() + correction * 0.5f);
                } else if (rbA) {
                    // Only A dynamic: apply full correction to A
                    trA->setPosition(trA->getPosition() - correction);
                } else if (rbB) {
                    // Only B dynamic: apply full correction to B
                    trB->setPosition(trB->getPosition() + correction);
                }
            }

            // Impulse resolution (equal mass assumption)
            if (rbA || rbB) {
                Vector3 va = rbA ? rbA->getVelocity() : Vector3{};
                Vector3 vb = rbB ? rbB->getVelocity() : Vector3{};
                Vector3 relVel = vb - va;

                float velAlongNormal = relVel.dot(m.normal);
                if (velAlongNormal > 0.f)
                    continue;

                float restitution = 0.5f;
                float j = -(1.f + restitution) * velAlongNormal;
                float invMassA = rbA ? 1.f : 0.f;
                float invMassB = rbB ? 1.f : 0.f;
                float invMassSum = invMassA + invMassB;
                if (invMassSum <= 0.f)
                    continue;
                j /= invMassSum;

                Vector3 impulse = m.normal * j;
                if (rbA)
                    rbA->setVelocity(va - impulse * invMassA);
                if (rbB)
                    rbB->setVelocity(vb + impulse * invMassB);
            }
        }
    }

    CollisionManifold PhysicsModule::computeOBBManifold(const OBB &a, const OBB &b) const {
        CollisionManifold m;
        Vector3 delta = b.center - a.center;

        printf("=== OBB Collision Check ===\n");
        printf("A center: (%.1f, %.1f, %.1f), halfExtents: (%.1f, %.1f, %.1f)\n",
               a.center.x, a.center.y, a.center.z,
               a.halfExtents.x, a.halfExtents.y, a.halfExtents.z);
        printf("B center: (%.1f, %.1f, %.1f), halfExtents: (%.1f, %.1f, %.1f)\n",
               b.center.x, b.center.y, b.center.z,
               b.halfExtents.x, b.halfExtents.y, b.halfExtents.z);
        printf("Delta: (%.1f, %.1f, %.1f)\n", delta.x, delta.y, delta.z);

        // 6 candidate axes (3 from each box)
        Vector3 axes[6] = {a.axes[0], a.axes[1], a.axes[2],
                           b.axes[0], b.axes[1], b.axes[2]};

        float minOverlap = (std::numeric_limits<float>::max)();
        Vector3 minAxis{};

        for (int i = 0; i < 6; ++i) {
            Vector3 axis = axes[i].normalized();
            float projA = projectExtent(a, axis);
            float projB = projectExtent(b, axis);
            
            // Skip degenerate axes where both OBBs have zero extent
            // (2D collision detection - boxes have zero thickness in Z)
            if (projA < 0.0001f && projB < 0.0001f) {
                printf("Axis %d: SKIPPED (degenerate - 2D box)\n", i);
                continue;
            }
            
            float centerDist = std::abs(axis.dot(delta));
            float overlap = projA + projB - centerDist;
            printf("Axis %d: projA=%.2f, projB=%.2f, centerDist=%.2f, overlap=%.2f\n",
                   i, projA, projB, centerDist, overlap);
            if (overlap <= 0.f) {
                printf(">>> SEPARATING AXIS FOUND at axis %d <<<\n\n", i);
                m.colliding = false;
                return m; // separating axis found
            }
            if (overlap < minOverlap) {
                minOverlap = overlap;
                // Ensure normal points from A to B
                minAxis = (axis.dot(delta) < 0.f) ? -axis : axis;
            }
        }

        printf(">>> COLLISION DETECTED! penetration=%.2f <<<\n\n", minOverlap);
        m.colliding = true;
        m.normal = minAxis;
        m.penetration = minOverlap;
        return m;
    }

    float PhysicsModule::projectExtent(const OBB &obb, const Vector3 &axis) const {
        return obb.halfExtents.x * std::abs(axis.dot(obb.axes[0])) +
               obb.halfExtents.y * std::abs(axis.dot(obb.axes[1])) +
               obb.halfExtents.z * std::abs(axis.dot(obb.axes[2]));
    }

} // namespace ParteeEngine