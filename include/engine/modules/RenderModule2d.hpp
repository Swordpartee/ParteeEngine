#pragma once

#include "Module.hpp"
#include "engine/Window.hpp"

namespace ParteeEngine {

    class Window;

    class RenderModule2d: public Module {
    friend class Engine;
    public:
        bool initialize(const ModuleInputs &inputs) override;
        bool update(const ModuleUpdateInputs &inputs) override;

        RenderModule2d(int width, int height) : Module(), window(width, height) {};

        Window window;
    };

    // Specialize trait
    template <>
    struct ModuleTraits<RenderModule2d> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Renderer;
    };
} // namespace ParteeEngine