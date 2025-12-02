#include "engine/RenderableFactory.hpp"

#include "engine/entities/components/RenderComponent.hpp"

namespace ParteeEngine {

    Entity& RenderableFactory::createSquare(Engine& engine) {
        Entity& entity = create(engine);

        RenderData data;
        data.sprite.textureID = 1; // Placeholder texture ID
        data.sprite.width = 50.0f;
        data.sprite.height = 50.0f;

        entity.getComponent<RenderComponent>()->renderData = data;

        return entity;
    }

    Entity& RenderableFactory::create(Engine &engine) {
        Entity& entity = engine.createEntity();
        entity.addComponent<RenderComponent>(std::unique_ptr<RenderComponent>(new RenderComponent(entity)));
        return entity;
    }

}
        