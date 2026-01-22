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

    private:
        static inline std::unordered_map<DeviceTypeID, std::vector<std::unique_ptr<InputDevice>>> devices;
        static inline std::mutex devicesMutex;
    };

} // namespace ParteeEngine