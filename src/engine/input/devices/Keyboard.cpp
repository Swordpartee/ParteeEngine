#include "engine/input/devices/Keyboard.hpp"

namespace ParteeEngine {

    bool Keyboard::isActive(InputBinding binding) const {
        // Treat inputID as a Windows virtual-key code directly (e.g., 'A', VK_SPACE).
        if (binding.inputID == 0) {
            return false;
        }

        const SHORT state = GetAsyncKeyState(static_cast<int>(binding.inputID));
        return (state & 0x8000) != 0;
    }

     float Keyboard::getAnalog(InputBinding binding) const {
        throw std::logic_error("Keyboard does not support analog inputs.");
    };
} // namespace ParteeEngine