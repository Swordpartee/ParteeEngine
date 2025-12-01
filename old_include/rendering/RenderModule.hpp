#pragma once

namespace ParteeEngine {
    struct RenderData {};

    struct Sprite::RenderData : public RenderData {
        int textureID;
        TransformComponent* transform;   
    };

    struct Mesh::RenderData : public RenderData {
        int meshID;
        TransformComponent* transform;
    };

    class RenderModule : public Module {
        virtual void init() override;

        virtual ~RenderModule() override;

        virtual ModuleOutput update(ModuleInput) override;

        virtual void render(const RenderData& data);

    };

}