#pragma once

#include "Module.hpp"
#include "engine/Window.hpp"

namespace ParteeEngine {

    class Window;

    class RenderModule3d: public Module {
    friend class Engine;
    public:
        void initialize(const ModuleInputs &inputs) override;
        void update(const ModuleUpdateInputs &inputs) override;

        RenderModule3d(int width = 800, int height = 600) : Module(), window(width, height) {};

        Window window;
    };

    // Specialize trait
    template <>
    struct ModuleTraits<RenderModule3d> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Renderer;
    };
} // namespace ParteeEngine