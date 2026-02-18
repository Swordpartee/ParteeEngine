#pragma once

#include "DeviceRegistry.hpp"

namespace ParteeEngine
{   
    using DeviceTypeID = uint16_t;

    class InputBinding;
    
    class InputDevice {
    public:
        virtual ~InputDevice() = default;

        virtual DeviceTypeID getDeviceTypeID() const = 0;

        virtual void poll() = 0;

        virtual bool isActive(InputBinding binding) const = 0;

        virtual float getAnalog(InputBinding binding) const = 0;
    protected:
        InputDevice() = default;
    };

    template<typename DerivedType>
    class InputDeviceCRTP : public InputDevice {
    public:
        static const DeviceTypeID typeID = DeviceRegistry::typeID<DerivedType>;

        DeviceTypeID getDeviceTypeID() const override {
            return typeID;
        }
    };

} // namespace ParteeEngine
