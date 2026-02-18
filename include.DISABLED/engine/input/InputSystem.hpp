#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

// #include "DeviceRegistry.hpp"
#include "InputDevice.hpp"
#include "InputBinding.hpp"

namespace ParteeEngine {
    

    class InputSystem {
    public:
        static void poll();

        static bool isActive(InputBinding binding);

        static float getAnalog(InputBinding binding);

        static void registerDevice(std::unique_ptr<InputDevice> device);

        static void registerInputEventSubscription(InputBinding binding);

    private:
        static inline std::unordered_map<InputBinding, bool> subscribedEventStates;
        static inline std::unordered_map<DeviceTypeID, std::vector<std::unique_ptr<InputDevice>>> devices;
        static inline std::mutex devicesMutex;

        static bool isActiveInternal(InputBinding binding);
    };

} // namespace ParteeEngine