#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"

int main() {
    ParteeEngine::Engine engine;

    engine.addModule<ParteeEngine::RenderModule2d>();

    ParteeEngine::Entity& entity = engine.createEntity();

    engine.run();

    return 0;
}