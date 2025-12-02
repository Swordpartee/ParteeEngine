#pragma once

namespace ParteeEngine {
    enum class ComponentCategory : unsigned
    {
        None = 0,
        Transform = 1 << 0,
        Render = 1 << 1,
        Physics = 1 << 2,
        Collider = 1 << 3
    };

    inline constexpr ComponentCategory operator|(ComponentCategory a, ComponentCategory b)
    {
        return static_cast<ComponentCategory>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }

    template <typename T>
    struct ComponentTraits {
        static constexpr bool unique = false;
        static constexpr ComponentCategory categories = ComponentCategory::None;
    };

} // namespace ParteeEngine