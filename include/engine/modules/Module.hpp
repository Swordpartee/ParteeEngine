#pragma once

namespace ParteeEngine {

    struct ModuleInputs;
    struct ModuleUpdateInputs;

    class Module {
    public:
        Module() = default;
        virtual ~Module() = default;

        virtual void initalize(ModuleInputs& inputs);

        virtual void update(ModuleUpdateInputs& inputs);
    };

} // namespace ParteeEngine