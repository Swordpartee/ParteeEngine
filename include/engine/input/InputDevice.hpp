#pragma once

#include <typeindex>

namespace parteeengine::input {   

    using DeviceType = std::type_index;

    struct InputBinding;
    
    class VirtualInputDevice {
    public:
        virtual ~VirtualInputDevice() = default;

        virtual void poll() = 0;

        virtual bool isActive(InputBinding binding) const = 0;

        virtual float getAnalog(InputBinding binding) const = 0;
    };

    template<typename Derived>
    class InputDeviceCRTP : public VirtualInputDevice {
    public:
        static DeviceType getId() {
            return typeid(Derived);
        }
    };

} // namespace parteeengine::input
