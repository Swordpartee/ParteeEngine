#pragma once

#include "engine/core/entities/Component.hpp"

namespace parteeengine {

    struct TestComponent: public Component {
        int value = 0;
    };

} // namespace parteeengine