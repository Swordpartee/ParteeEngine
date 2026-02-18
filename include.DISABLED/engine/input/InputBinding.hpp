#pragma once

namespace ParteeEngine {

    struct InputBinding {
        DeviceTypeID deviceID;
        uint16_t inputID;
        
        size_t deviceIndex = 0; // For multiple devices of the same type

        bool operator==(const InputBinding& other) const {
            return deviceID == other.deviceID &&
                   deviceIndex == other.deviceIndex &&
                   inputID == other.inputID;
        }
    };
}

#include <functional>

namespace std {
    template<>
    struct hash<ParteeEngine::InputBinding> {
        size_t operator()(const ParteeEngine::InputBinding& b) const {
            size_t h1 = hash<ParteeEngine::DeviceTypeID>()(b.deviceID);
            size_t h2 = hash<size_t>()(b.deviceIndex);
            size_t h3 = hash<uint16_t>()(b.inputID);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}