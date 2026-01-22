#include "engine/input/InputSystem.hpp"

namespace ParteeEngine {

    void InputSystem::poll() {
        std::lock_guard lock(devicesMutex);
        for (auto& [typeID, deviceList] : devices) {
            for (auto& device : deviceList) {
                device->poll();
            }
        }
    }

    bool InputSystem::isActive(InputBinding binding) {
        std::lock_guard lock(devicesMutex);
        auto it = devices.find(binding.deviceID);
        if (it != devices.end()) {
            const auto& deviceList = it->second;
            if (binding.deviceIndex >= 0 && static_cast<size_t>(binding.deviceIndex) < deviceList.size()) {
                return deviceList[static_cast<size_t>(binding.deviceIndex)]->isActive(binding);
            } else {
                throw std::out_of_range("Device index out of range for device");
            }
        } else {
            throw std::invalid_argument("Device ID not found in registered devices");
        }
        return false;
    }

    float InputSystem::getAnalog(InputBinding binding) {
        std::lock_guard lock(devicesMutex);
        auto it = devices.find(binding.deviceID);
        if (it != devices.end()) {
            const auto& deviceList = it->second;
            if (binding.deviceIndex >= 0 && static_cast<size_t>(binding.deviceIndex) < deviceList.size()) {
                return deviceList[static_cast<size_t>(binding.deviceIndex)]->getAnalog(binding);
            }
        }
        return 0.0f;
    }

    void InputSystem::registerDevice(std::unique_ptr<InputDevice> device) {
        if (!device) {
            return;
        }

        DeviceTypeID typeID = device->getDeviceTypeID();
        std::lock_guard lock(devicesMutex);
        devices[typeID].push_back(std::move(device));
    }

} // namespace ParteeEngine