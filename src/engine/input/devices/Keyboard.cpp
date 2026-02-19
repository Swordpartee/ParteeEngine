#include "engine/input/devices/Keyboard.hpp"

namespace parteeengine::input {

    bool Keyboard::isActive(InputBinding binding) const {
        // Treat inputID as a Windows virtual-key code directly (e.g., 'A', VK_SPACE).
        if (binding.id == 0) {
            return false;
        }

        const SHORT state = GetAsyncKeyState(static_cast<int>(binding.id));
        return (state & 0x8000) != 0;
    }

     float Keyboard::getAnalog([[maybe_unused]]InputBinding binding) const {
        throw std::logic_error("Keyboard does not support analog inputs.");
    };
    
} // namespace parteeengine::input