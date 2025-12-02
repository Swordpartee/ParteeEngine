#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/entities/components/RenderComponent.hpp"
#include "engine/RenderableFactory.hpp"

int main() {

    ParteeEngine::Engine engine;

    engine.addModule<ParteeEngine::RenderModule2d>();

    ParteeEngine::Entity& entity = ParteeEngine::RenderableFactory::createSquare(engine);

    entity.getComponent<ParteeEngine::TransformComponent>()->setPosition({200.0f, 150.0f, 0.0f});

    engine.run();

    return 0;
}