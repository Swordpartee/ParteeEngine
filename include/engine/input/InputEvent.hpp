#pragma once

#include "InputBinding.hpp"
#include "engine/events/Event.hpp"
#include "InputSystem.hpp"

#include <cstdint>

namespace ParteeEngine {

    struct InputEvent : public Event { 
    public:
        InputEvent(const InputBinding& binding, bool pressed)
            : binding(binding), pressed(pressed) {}

        InputBinding binding;
        bool pressed;

        void subscribed() override {
            InputSystem::registerInputEventSubscription(binding);
        }

        bool operator==(const InputEvent& other) const {
            // const InputEvent* otherEvent = dynamic_cast<const InputEvent*>(&other);
            
            return binding == other.binding && pressed == other.pressed;
        }
    };
    
} // namespace ParteeEngine

// Hash specialization for InputBinding
#include <functional>