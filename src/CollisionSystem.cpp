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
            case ColliderType::SQUARE:
                switch(collider2->getType()) 
                {
                    case ColliderType::SQUARE:
                        resolveSquareSquareCollision(e1, e2);
                        break;
                    case ColliderType::CIRCLE:
                        resolveSquareCircleCollision(e1, e2);
                        break;
                }
                break;
            case ColliderType::CIRCLE:
                switch(collider2->getType()) 
                {
                    case ColliderType::SQUARE:
                        resolveSquareCircleCollision(e2, e1);
                        break;
                    case ColliderType::CIRCLE:
                        resolveCircleCircleCollision(e1, e2);
                        break;
                }
                break;
        }
    }

    void CollisionSystem::resolveSquareSquareCollision(Entity &e1, Entity &e2) 
    {
        ColliderData& data1 = e1.getComponent<ColliderComponent>()->getData();
        ColliderData& data2 = e2.getComponent<ColliderComponent>()->getData();

        TransformComponent* transform1 = e1.getComponent<TransformComponent>();
        TransformComponent* transform2 = e2.getComponent<TransformComponent>();

        if (transform1 && transform2) {
            float x1 = transform1->position.x;
            float y1 = transform1->position.y;
            float x2 = transform2->position.x;
            float y2 = transform2->position.y;

            float left1 = x1 - data1.square.halfExtents.x;
            float right1 = x1 + data1.square.halfExtents.x;
            float top1 = y1 - data1.square.halfExtents.y;
            float bottom1 = y1 + data1.square.halfExtents.y;
            
            float left2 = x2 - data2.square.halfExtents.x;
            float right2 = x2 + data2.square.halfExtents.x;
            float top2 = y2 - data2.square.halfExtents.y;
            float bottom2 = y2 + data2.square.halfExtents.y;
            
            if (right1 > left2 && left1 < right2 && bottom1 > top2 && top1 < bottom2) {
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
            float sx = transform1->position.x;
            float sy = transform1->position.y;
            float cx = transform2->position.x;
            float cy = transform2->position.y;
            
            float left = sx - data1.square.halfExtents.x;
            float right = sx + data1.square.halfExtents.x;
            float top = sy - data1.square.halfExtents.y;
            float bottom = sy + data1.square.halfExtents.y;
            
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
            float dx = transform2->position.x - transform1->position.x;
            float dy = transform2->position.y - transform1->position.y;
            float distSquared = dx * dx + dy * dy;
            float radiusSum = data1.circle.radius + data2.circle.radius;
            
            if (distSquared < radiusSum * radiusSum) {
                EventBus::getInstance().publish(CollisionEvent{e1.getID(), e2.getID()});
            }
        }
    }
}