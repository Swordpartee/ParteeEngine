#pragma once

#include "Module.hpp"

namespace ParteeEngine {

    class Window;

    class RenderModule2d: public Module {
    friend class Engine;
    public:
        void initialize(const ModuleInputs &inputs) override;
        void update(const ModuleUpdateInputs &inputs) override;

    private:
        using Module::Module;

        Window* window;
    };

    // Specialize trait
    template <>
    struct ModuleTraits<RenderModule2d> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Renderer;
    };
} // namespace ParteeEngine