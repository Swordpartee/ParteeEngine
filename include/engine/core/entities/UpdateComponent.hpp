#pragma once

#include "engine/core/entities/Component.hpp"

namespace ParteeEngine {

    class UpdateComponent : public Component {
    public:
        UpdateComponent() = default;
        ~UpdateComponent() override = default;

        void requireDependencies() override {}

        void onAttach() override {}
        void onDetach() override {}
        void onUpdate(float dt) override {
            if (updateFunction) {
                updateFunction(dt, owner);
            }
        }

        void setUpdateFunction(std::function<void(float, Entity*)> func) {
            updateFunction = func;
        }

    private:
        std::function<void(float, Entity*)> updateFunction;
    };
   
} // namespace ParteeEngine