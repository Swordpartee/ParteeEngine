#include "engine/modules/PhysicsModule.hpp"

#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/math/Vector3.hpp"

namespace ParteeEngine {

    void PhysicsModule::initialize(const ModuleInputs& inputs) {
        
    }

    void PhysicsModule::update(const ModuleUpdateInputs& inputs) {
        // Update physics for each entity with a RigidBodyComponent
        for (auto& entity : inputs.entities) {
            auto* rigidBody = entity.getComponent<RigidBodyComponent>();
            if (!rigidBody) continue;

            auto* transform = entity.getComponent<TransformComponent>();
            if (!transform) continue;

            Vector3 newPosition = transform->getPosition() + rigidBody->getVelocity();
        }
    }

} // namespace ParteeEngine