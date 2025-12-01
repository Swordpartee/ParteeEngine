#include "CollisionSystem.hpp"

#include "components/ColliderComponent.hpp"
#include "Entity.hpp"
#include "events/EventBus.hpp"

namespace ParteeEngine 
{
    void CollisionSystem::updateCollisions(std::vector<Entity>& entities) 
    {
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                checkCollision(entities[i], entities[j]);
            }
        }
    }

    void CollisionSystem::checkCollision(Entity& e1, Entity& e2) 
    {
        auto collider1 = e1.getComponent<ColliderComponent>();
        auto collider2 = e2.getComponent<ColliderComponent>();
        if (!collider1 || !collider2) return;

        switch(collider1->getType()) 
        {
            case ColliderType::CUBE:
                switch(collider2->getType()) 
                {
                    case ColliderType::CUBE:
                        resolveCubeCubeCollision(e1, e2);
                        break;
                    case ColliderType::CIRCLE:
                        resolveSquareCircleCollision(e1, e2);
                        break;
                }
                break;
            case ColliderType::CIRCLE:
                switch(collider2->getType()) 
                {
                    case ColliderType::CUBE:
                        resolveSquareCircleCollision(e2, e1);
                        break;
                    case ColliderType::CIRCLE:
                        resolveCircleCircleCollision(e1, e2);
                        break;
                }
                break;
        }
    }

    void CollisionSystem::resolveCubeCubeCollision(Entity &e1, Entity &e2) 
    {
        ColliderData& data1 = e1.getComponent<ColliderComponent>()->getData();
        ColliderData& data2 = e2.getComponent<ColliderComponent>()->getData();

        TransformComponent* transform1 = e1.getComponent<TransformComponent>();
        TransformComponent* transform2 = e2.getComponent<TransformComponent>();

        if (transform1 && transform2) {
            if (std::abs(transform1->getPosition().x - transform2->getPosition().x) <= (data1.cube.halfExtents.x + data2.cube.halfExtents.x) &&
                std::abs(transform1->getPosition().y - transform2->getPosition().y) <= (data1.cube.halfExtents.y + data2.cube.halfExtents.y) &&
                std::abs(transform1->getPosition().z - transform2->getPosition().z) <= (data1.cube.halfExtents.z + data2.cube.halfExtents.z)) {
                EventBus::getInstance().publish(CollisionEvent{e1.getID(), e2.getID()});
            }
        }

    }
    void CollisionSystem::resolveSquareCircleCollision(Entity &e1, Entity &e2) 
    {
        ColliderData& data1 = e1.getComponent<ColliderComponent>()->getData();
        ColliderData& data2 = e2.getComponent<ColliderComponent>()->getData();

        TransformComponent* transform1 = e1.getComponent<TransformComponent>();
        TransformComponent* transform2 = e2.getComponent<TransformComponent>();

        if (transform1 && transform2) {
            float sx = transform1->getPosition().x;
            float sy = transform1->getPosition().y;
            float cx = transform2->getPosition().x;
            float cy = transform2->getPosition().y;

            float left = sx - data1.cube.halfExtents.x;
            float right = sx + data1.cube.halfExtents.x;
            float top = sy - data1.cube.halfExtents.y;
            float bottom = sy + data1.cube.halfExtents.y;
            
            float closestX = std::max(left, std::min(cx, right));
            float closestY = std::max(top, std::min(cy, bottom));
            
            float distX = cx - closestX;
            float distY = cy - closestY;
            float distSquared = distX * distX + distY * distY;
            
            if (distSquared < data2.circle.radius * data2.circle.radius) {
                EventBus::getInstance().publish(CollisionEvent{e1.getID(), e2.getID()});
            }
        }
    }
    void CollisionSystem::resolveCircleCircleCollision(Entity &e1, Entity &e2) 
    {
        ColliderData& data1 = e1.getComponent<ColliderComponent>()->getData();
        ColliderData& data2 = e2.getComponent<ColliderComponent>()->getData();

        TransformComponent* transform1 = e1.getComponent<TransformComponent>();
        TransformComponent* transform2 = e2.getComponent<TransformComponent>();

        if (transform1 && transform2) {
            float dx = transform2->getPosition().x - transform1->getPosition().x;
            float dy = transform2->getPosition().y - transform1->getPosition().y;
            float distSquared = dx * dx + dy * dy;
            float radiusSum = data1.circle.radius + data2.circle.radius;
            
            if (distSquared < radiusSum * radiusSum) {
                EventBus::getInstance().publish(CollisionEvent{e1.getID(), e2.getID()});
            }
        }
    }
}