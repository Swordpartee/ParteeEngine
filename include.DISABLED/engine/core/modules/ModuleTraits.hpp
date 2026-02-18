#pragma once

namespace ParteeEngine {

    enum class ModuleCategory : unsigned {
        None = 0,
        Renderer = 1u << 0,
        Physics = 1u << 1,
        Audio = 1u << 2,
        Input = 1u << 3
    };

    inline constexpr ModuleCategory operator|(ModuleCategory a, ModuleCategory b)
    {
        return static_cast<ModuleCategory>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }

    // Default trait (no category, not unique)
    template <typename T>
    struct ModuleTraits
    {
        static constexpr bool unique = false;
        static constexpr ModuleCategory categories = ModuleCategory::None;
    };

} // namespace ParteeEngine