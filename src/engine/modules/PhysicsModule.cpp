#include "engine/modules/PhysicsModule.hpp"

#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/entities/components/ColliderComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/math/Vector3.hpp"
#include <cmath>
#include <cfloat>
#include <algorithm>

namespace ParteeEngine {

    void PhysicsModule::initialize(const ModuleInputs& inputs) {
        // Initialize physics settings
    }

    void PhysicsModule::update(const ModuleUpdateInputs& inputs) {
        // Claculate physics for all entities with rigidbodies
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

    void PhysicsModule::checkCollisions(std::vector<Entity>& entities) {
        // Broadphase collision detection using bounding spheres
        std::vector<std::pair<Entity*, Entity*>> potentialCollisions;

        for (size_t i = 0; i < entities.size(); ++i) {
            auto* colliderA = entities[i].getComponent<ColliderComponent>();
            if (!colliderA) continue;

            for (size_t j = i + 1; j < entities.size(); ++j) {
                auto* colliderB = entities[j].getComponent<ColliderComponent>();
                if (!colliderB) continue;

                // Simple distance check between entity positions
                auto* transformA = entities[i].getComponent<TransformComponent>();
                auto* transformB = entities[j].getComponent<TransformComponent>();
                if (!transformA || !transformB) continue;

                Vector3 posA = transformA->getPosition();
                Vector3 posB = transformB->getPosition();

                float distSq = (posA - posB).lengthSquared();

                float radiusSum = colliderA->getBoundingSphereRadius() + colliderB->getBoundingSphereRadius();
                
                if (distSq <= radiusSum * radiusSum) {
                    potentialCollisions.emplace_back(&entities[i], &entities[j]);

                    std::printf(
                        "Checking collision between Entity %zu at (%.2f, %.2f, "
                        "%.2f) and Entity %zu at (%.2f, %.2f, %.2f)\n",
                        i, posA.x, posA.y, posA.z, j, posB.x, posB.y, posB.z);

                    std::fflush(stdout);
                }
            }
        }

        // Narrowphase collision detection would go here (e.g., OBB vs OBB)
        // For now, we just log potential collisions
        for (const auto& pair : potentialCollisions) {
            Entity* entityA = pair.first;
            Entity* entityB = pair.second;
            // Handle collision response (e.g., simple velocity inversion)
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