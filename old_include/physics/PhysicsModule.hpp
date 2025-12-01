#pragma once

namespace ParteeEngine {
    class PhysicsModule {
        void init() override;

        ~PhysicsModule() override;

        ModuleOutput update(ModuleInput&) override;
    };

} // namespace ParteeEngine