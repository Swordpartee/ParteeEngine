#include "engine/input/InputSystem.hpp"

namespace parteeengine::input {

    void InputSystem::poll() {
        for (auto& [type, deviceList] : devices) {
            for (auto& device : deviceList) {
                device->poll();
            }
        }
    }

    bool InputSystem::isActive(InputBinding binding) {
        auto deviceList = devices.find(binding.type);
        if (deviceList != devices.end() && binding.deviceIndex < deviceList->second.size()) {
            return deviceList->second[binding.deviceIndex]->isActive(binding);
        }
        return false;
    }

    float InputSystem::getAnalog(InputBinding binding) {
        auto deviceList = devices.find(binding.type);
        if (deviceList != devices.end() && binding.deviceIndex < deviceList->second.size()) {
            return deviceList->second[binding.deviceIndex]->getAnalog(binding);
        }
        return 0.f;
    }

} // namespace parteeengine::input