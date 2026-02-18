#pragma once

namespace parteeengine {

    struct ModuleInput {
        
    };

    class Module {
    public:
        virtual ~Module() = default;

        virtual bool initialize(const ModuleInput& input) = 0;
        virtual bool update(const ModuleInput& input) = 0;

        bool requestStop() {
            return stopRequested;
        }

    private:
        bool stopRequested = false;
    };

} // namespace parteeengine