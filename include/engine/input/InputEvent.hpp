// #pragma once

// #include "InputBinding.hpp"
// #include "engine/events/Event.hpp"
// #include "InputSystem.hpp"

// #include <cstdint>

// namespace parteeengine::input {

//     struct InputEvent : public Event { 
//     public:
//         InputEvent(const InputBinding& binding, bool pressed)
//             : binding(binding), pressed(pressed) {}

//         InputBinding binding;
//         bool pressed;

//         void subscribed() override {
//             InputSystem::registerInputEventSubscription(binding);
//         }

//         bool operator==(const InputEvent& other) const {            
//             return binding == other.binding && pressed == other.pressed;
//         }
//     };
    
// } // namespace parteeengine::input
