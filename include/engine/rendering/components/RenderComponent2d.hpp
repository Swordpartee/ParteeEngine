#pragma once

#include "engine/core/entities/Component.hpp"

namespace ParteeEngine::Rendering {

    template<typename T>
    concept IsRenderCommand = std::is_base_of_v<RenderCommand, T>;

    template<IsRenderCommand T>
    class RenderComponent2d : public ParteeEngine::Component {
    public:
        RenderComponent2d() = default;
        ~RenderComponent2d() override = default;

        void requireDependencies() override {}

        void onAttach() override {}
        void onDetach() override {}
        void onUpdate(float dt) override {}

        T& getCommand() { return command; }
        const T& getCommand() const { return command; }

    protected:
        T command;
    };

} // namespace ParteeEngine::Rendering