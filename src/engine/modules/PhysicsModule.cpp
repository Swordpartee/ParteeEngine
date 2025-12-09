#include "engine/modules/PhysicsModule.hpp"

#include "engine/entities/Entity.hpp"
#include "engine/entities/components/ColliderComponent.hpp"
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
        rigidbody->setVelocity(velocity);

        // Update position based on velocity
        Vector3 position = transform->getPosition();
        position += velocity * dt;
        transform->setPosition(position);
    }

    // Helper: Check if two AABBs (Axis-Aligned Bounding Boxes) intersect
    bool checkAABBCollision(const Vector3& minA, const Vector3& maxA,
                           const Vector3& minB, const Vector3& maxB) {
        return (minA.x <= maxB.x && maxA.x >= minB.x) &&
               (minA.y <= maxB.y && maxA.y >= minB.y) &&
               (minA.z <= maxB.z && maxA.z >= minB.z);
    }

    // Helper: Get AABB bounds for a box collider
    void getAABBBounds(Entity& entity, Vector3& outMin, Vector3& outMax) {
        auto* transform = entity.getComponent<TransformComponent>();
        Vector3 position = transform->getPosition();
        Vector3 scale = transform->getScale();
        Vector3 halfExtents = scale * 0.5f;
        
        outMin = position - halfExtents;
        outMax = position + halfExtents;
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

        // Narrowphase: Precise AABB collision detection
        std::vector<std::pair<Entity*, Entity*>> actualCollisions;
        
        for (const auto& pair : potentialCollisions) {
            Entity* entityA = pair.first;
            Entity* entityB = pair.second;

            // Check if both are box colliders
            auto* boxA = dynamic_cast<BoxColliderComponent*>(entityA->getComponent<ColliderComponent>());
            auto* boxB = dynamic_cast<BoxColliderComponent*>(entityB->getComponent<ColliderComponent>());

            if (boxA && boxB) {
                Vector3 minA, maxA, minB, maxB;
                getAABBBounds(*entityA, minA, maxA);
                getAABBBounds(*entityB, minB, maxB);

                if (checkAABBCollision(minA, maxA, minB, maxB)) {
                    actualCollisions.push_back(pair);
                }
            }
        }

        // Collision response
        for (const auto& pair : actualCollisions) {
            Entity* entityA = pair.first;
            Entity* entityB = pair.second;

            // Handle collision response (simple velocity inversion)
            auto* rigidbodyA = entityA->getComponent<RigidBodyComponent>();
            auto* rigidbodyB = entityB->getComponent<RigidBodyComponent>();
            
            if (rigidbodyA) {
                Vector3 velA = rigidbodyA->getVelocity();
                rigidbodyA->setVelocity(Vector3(-velA.x, -velA.y, -velA.z));
            }
            if (rigidbodyB) {
                Vector3 velB = rigidbodyB->getVelocity();
                rigidbodyB->setVelocity(Vector3(-velB.x, -velB.y, -velB.z));
            }
        }
    }

} // namespace ParteeEngine