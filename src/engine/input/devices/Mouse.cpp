#include "engine/input/devices/Mouse.hpp"

namespace parteeengine::input {

    bool Mouse::isActive(InputBinding binding) const {
        // Map inputID to mouse buttons
        switch (binding.id) {
            case 0: // Left button
                return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
            case 1: // Right button
                return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
            case 2: // Middle button
                return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
            default:
                return false;
        }
    }

    float Mouse::getAnalog(InputBinding binding) const {
        POINT cursorPos;
        if (!GetCursorPos(&cursorPos)) {
            throw std::runtime_error("Failed to get cursor position.");
        }

        switch (binding.id) {
            case 3: // X position
                return static_cast<float>(cursorPos.x);
            case 4: // Y position
                return static_cast<float>(cursorPos.y);
            case 5: { // Wheel delta
                // Note: This requires handling WM_MOUSEWHEEL messages in a real application.
                throw std::logic_error("Mouse wheel delta not implemented.");
            }
            default:
                throw std::invalid_argument("Invalid analog input ID for Mouse.");
        }
    } 

} // namespace parteeengine::input