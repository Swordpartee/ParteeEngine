#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"

namespace ParteeEngine {

    class Vector3;

    class ColliderComponent: public Component {    
    public:
        virtual ~ColliderComponent() = default;

        void ensureDependencies() override {
            owner.ensureComponent<TransformComponent>();
        };

        float getBoundingSphereRadius() const { return boundingSphereRadius; }
        

    protected:
        using Component::Component;
        void setBoundingSphereRadius(float radius) { boundingSphereRadius = radius; }
    
    private:
        float boundingSphereRadius;
    };

    class BoxColliderComponent : public ColliderComponent {
    friend class Entity;

        void onAttach() override {
            auto* transform = owner.getComponent<TransformComponent>();
            Vector3 scale = transform->getScale();
            // Assuming the box collider is axis-aligned and centered on the entity
            Vector3 halfExtents = scale * 0.5f;
            // Calculate bounding sphere radius as half the diagonal of the box
            float radius = halfExtents.length();
            setBoundingSphereRadius(radius);
        };
    
    private:
        using ColliderComponent::ColliderComponent;
    };

    template <>
    struct ComponentTraits<ColliderComponent>
    {
        static constexpr bool unique = false;
        static constexpr ComponentCategory categories = ComponentCategory::Collider;
    };

    template <>
    struct ComponentTraits<BoxColliderComponent>
    {
        static constexpr bool unique = true;
        static constexpr ComponentCategory categories = ComponentCategory::Collider;
    };

} // namespace ParteeEngine