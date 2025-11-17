#include "components/ColliderComponent.hpp"

namespace ParteeEngine 
{
    ColliderComponent::ColliderComponent(ColliderType type) : type(type)
    {
        if (type == ColliderType::SQUARE) 
        {
            data.square.halfExtents = Vector3(0.5f, 0.5f, 0.5f); // Default half extents
        } 
        else if (type == ColliderType::CIRCLE) 
        {
            data.circle.radius = 1.0f; // Default radius
        }
    }

    void ColliderComponent::requireDependencies(Entity &owner) 
    {
        owner.ensureComponent<TransformComponent>();
    }

    ColliderType ColliderComponent::getType() const 
    {
        return type; 
    }

    ColliderData& ColliderComponent::getData() 
    {
        return data;
    }
    
} //namespace ParteeEngine