#pragma once

#include "components/Component.hpp"
#include "components/TransformComponent.hpp"
#include "components/ColliderComponent.hpp"

namespace ParteeEngine 
{
    class Entity; // Forward declaration

    class CollisionSystem 
    {
        public:
            void updateCollisions(std::vector<Entity>& entities);

        private:
            void checkCollision(Entity &e1, Entity &e2);

            void resolveSquareSquareCollision(Entity &e1, Entity &e2);
            void resolveSquareCircleCollision(Entity &e1, Entity &e2);
            void resolveCircleCircleCollision(Entity &e1, Entity &e2);
    };

   

} //namespace ParteeEngine