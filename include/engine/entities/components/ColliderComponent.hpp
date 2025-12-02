#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"

namespace ParteeEngine {

    class Vector3;

    class ColliderComponent: public Component {
    friend class Entity;
    
    public:
        virtual ~ColliderComponent() = default;

        void onAttach() override {};
        void ensureDependencies() override {
            owner.ensureComponent<TransformComponent>();
        };

        bool getIsTrigger() const { return isTrigger; }
        void setIsTrigger(bool trigger) { isTrigger = trigger; }

    protected:
        using Component::Component;
    
    private:
        bool isTrigger = false;
    };

    class BoxColliderComponent : public ColliderComponent {
    public:
        using ColliderComponent::ColliderComponent;

        Vector3 getHalfSize() const { return halfSize; }
        void setHalfSize(const Vector3& size) { halfSize = size; }

        Vector3 getOffset() const { return offset; }
        void setOffset(const Vector3& off) { offset = off; }
    
    private:
        Vector3 halfSize{16.f, 16.f, 0.f};
        Vector3 offset{0.f, 0.f, 0.f};
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