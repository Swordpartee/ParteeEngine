#pragma once

#include <functional>

namespace parteeengine::input {

    using DeviceType = std::type_index;
    using BindingID = uint16_t;

    struct InputBinding {
        DeviceType type;
        BindingID id;
        size_t deviceIndex = 0; // For multiple devices of the same type

        bool operator==(const InputBinding& other) const {
            return type == other.type && id == other.id && deviceIndex == other.deviceIndex;
        }

        InputBinding(DeviceType type, BindingID id, size_t deviceIndex = 0) : type(type), id(id), deviceIndex(deviceIndex) {}
    };
} // namespace parteeengine::input