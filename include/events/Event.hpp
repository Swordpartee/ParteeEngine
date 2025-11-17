#pragma once
#include <typeindex>

namespace ParteeEngine {
    class Entity;

    struct Event {
        virtual ~Event() = default;
    };

    struct CollisionEvent : public Event{
        public:
            CollisionEvent(const Entity& first, const Entity& second) : first_(first), second_(second) {}
            const Entity &getFirst() { return first_; }
            const Entity &getSecond() { return second_; }

        private:
            const Entity& first_;
            const Entity& second_;
    };
}