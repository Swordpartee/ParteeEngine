#pragma once

namespace ParteeEngine {

    struct InputBinding {
        DeviceTypeID deviceID;
        uint16_t inputID;
        
        int deviceIndex = 0; // For multiple devices of the same type

        bool operator==(const InputBinding& other) const {
            return deviceID == other.deviceID && deviceIndex == other.deviceIndex && inputID == other.inputID;
        }
    };
}