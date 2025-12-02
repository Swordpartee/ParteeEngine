#pragma once

#include "Module.hpp"

namespace ParteeEngine {

    class Vector3;

    class PhysicsModule : public Module {
    public:
        ~PhysicsModule() override = default;

        void initialize(const ModuleInputs& inputs) override;

        void update(const ModuleUpdateInputs& inputs) override;
    };

    template <>
    struct ModuleTraits<PhysicsModule> {
        static constexpr bool unique = true;
        static constexpr ModuleCategory categories = ModuleCategory::Physics;
    };

} // namespace ParteeEngine