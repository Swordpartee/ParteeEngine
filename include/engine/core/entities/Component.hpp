#pragma once

#include <typeindex>

namespace parteeengine {

    struct VirtualComponent {
    };

    template<typename Derived>
    struct ComponentCRTP : public VirtualComponent {
        static std::type_index getType() {
            return typeid(Derived);
        }
    };


} // namespace parteeengine