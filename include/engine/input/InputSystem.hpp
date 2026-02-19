#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

#include "InputDevice.hpp"
#include "InputBinding.hpp"

namespace parteeengine::input {
    
    class InputSystem {
    public:
        static void poll();

        static bool isActive(InputBinding binding);

        static float getAnalog(InputBinding binding);

        template<typename DeviceType>
        static void registerDevice();

    private:
        static inline std::unordered_map<DeviceType, std::vector<std::unique_ptr<VirtualInputDevice>>> devices;

    };

    template<typename DeviceType>
    void InputSystem::registerDevice() {
        devices[typeid(DeviceType)].push_back(std::make_unique<DeviceType>());
    }

} // namespace parteeengine::input