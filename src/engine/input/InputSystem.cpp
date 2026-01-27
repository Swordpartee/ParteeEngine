#include "engine/input/InputSystem.hpp"

#include "engine/events/EventBus.hpp"
#include "engine/input/InputEvent.hpp"

namespace ParteeEngine {

    void InputSystem::poll() {
        std::lock_guard lock(devicesMutex);
        for (auto& [typeID, deviceList] : devices) {
            for (auto& device : deviceList) {
                device->poll();
            }
        }
        for (auto& [binding, previousState] : subscribedEventStates) {
            if (isActiveInternal(binding) != previousState) {
                // State changed, publish event
                EventBus::publish<InputEvent>(InputEvent{binding, !previousState});
                previousState = !previousState;
            }
        }
    }

    bool InputSystem::isActive(InputBinding binding) {
        std::lock_guard lock(devicesMutex);
        auto device = devices.find(binding.deviceID);
        return isActiveInternal(binding);
       
    }

    float InputSystem::getAnalog(InputBinding binding) {
        std::lock_guard lock(devicesMutex);
        auto it = devices.find(binding.deviceID);
        if (it == devices.end()) {
            throw std::invalid_argument("Device type not found for analog input.");
        }
        const auto& deviceList = it->second;
        if (binding.deviceIndex >= deviceList.size()) {
            throw std::out_of_range("Device index out of range for analog input.");
        }
        return deviceList[binding.deviceIndex]->getAnalog(binding);
    }

    void InputSystem::registerDevice(std::unique_ptr<InputDevice> device) {
        if (!device) {
            return;
        }

        DeviceTypeID typeID = device->getDeviceTypeID();
        std::lock_guard lock(devicesMutex);
        devices[typeID].push_back(std::move(device));
    }

    void InputSystem::registerInputEventSubscription(InputBinding binding) {
        auto it = devices.find(binding.deviceID);
        if (it == devices.end()) {
            throw std::invalid_argument("Device type not found.");
        }
        subscribedEventStates[binding] = isActive(binding);
    }

    bool InputSystem::isActiveInternal(InputBinding binding) {
        auto device = devices.find(binding.deviceID);
        if (device == devices.end()) {
            throw std::invalid_argument("Device type not found.");
        }
        const auto& deviceList = device->second;
        if (binding.deviceIndex >= deviceList.size()) {
            throw std::out_of_range("Device index out of range.");
        }
        return deviceList[binding.deviceIndex]->isActive(binding);
    }

} // namespace ParteeEngine