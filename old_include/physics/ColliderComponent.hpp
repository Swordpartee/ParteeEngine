#pragma once

#include "Component.hpp"
#include "components/TransformComponent.hpp"
#include "Entity.hpp"

namespace ParteeEngine 
{
    class Entity; // Forward declaration

    enum class ColliderType 
    {
        CUBE,
        CIRCLE
    };

    struct ColliderData 
    {   
        struct {
            Vector3 halfExtents; // For SQUARE
        } cube;
        struct {
            float radius; // For CIRCLE
        } circle;
    };

    class ColliderComponent : public Component 
    {
        public:
            ColliderComponent(ColliderType type);

            void requireDependencies(Entity &owner) override;

            ColliderType getType() const;

            ColliderData& getData();

        private:
            ColliderType type;
            ColliderData data;
            
    };

} //namespace ParteeEngine