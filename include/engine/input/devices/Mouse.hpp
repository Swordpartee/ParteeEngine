#pragma once

#include <Windows.h>
#include <stdexcept>

#include "engine/input/InputDevice.hpp"
#include "engine/input/InputBinding.hpp"

namespace parteeengine::input {

    class Mouse : public InputDeviceCRTP<Mouse> {
    public:
        Mouse() = default;
        ~Mouse() override = default;

        bool isActive(InputBinding binding) const override;

        float getAnalog(InputBinding binding) const override;
    };

    namespace Bindings {
        inline const InputBinding MouseButtonLeft{Mouse::getId(), 0};
        inline const InputBinding MouseButtonRight{Mouse::getId(), 1};
        inline const InputBinding MouseButtonMiddle{Mouse::getId(), 2};

        inline const InputBinding MouseX{Mouse::getId(), 3};
        inline const InputBinding MouseY{Mouse::getId(), 4};
        inline const InputBinding MouseWheel{Mouse::getId(), 5};
    }

} // namespace parteeengine::input