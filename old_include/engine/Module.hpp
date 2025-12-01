#pragma once

namespace ParteeEngine {
    struct ModuleInput;
    struct ModuleOutput;

    class Module {
        virtual void init() {}
        
        virtual ~Module() = default;

        virtual ModuleOutput update(ModuleInput&);
    }
} // namespace ParteeEngine