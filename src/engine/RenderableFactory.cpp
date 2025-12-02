#include "engine/RenderableFactory.hpp"

namespace ParteeEngine {

    Entity& RenderableFactory::createSquare(Engine& engine) {
        Entity& entity = create(engine);
        return entity;
    }

    Entity& RenderableFactory::create(Engine &engine) {
        Entity& entity = engine.createEntity();
        entity.addComponent<RenderComponent>(std::unique_ptr<RenderComponent>(new RenderComponent(entity)));
        return entity;
    }

}
        