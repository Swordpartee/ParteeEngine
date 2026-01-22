#pragma once

#include <Windows.h>
#include <stdexcept>

#include "engine/input/InputDevice.hpp"
#include "engine/input/InputBinding.hpp"

namespace ParteeEngine {

    class Mouse : public InputDeviceCRTP<Mouse> {
    public:
        Mouse() = default;
        ~Mouse() override = default;

        bool isActive(InputBinding binding) const override;

        float getAnalog(InputBinding binding) const override;
    };

    namespace Bindings {
        inline constexpr InputBinding MouseButtonLeft{Mouse::typeID, 0};
        inline constexpr InputBinding MouseButtonRight{Mouse::typeID, 1};
        inline constexpr InputBinding MouseButtonMiddle{Mouse::typeID, 2};

        inline constexpr InputBinding MouseX{Mouse::typeID, 3};
        inline constexpr InputBinding MouseY{Mouse::typeID, 4};
        inline constexpr InputBinding MouseWheel{Mouse::typeID, 5};
    }

} // namespace ParteeEngine