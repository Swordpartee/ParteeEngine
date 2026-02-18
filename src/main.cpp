#include "engine/core/Engine.hpp"
#include "engine/core/modules/TestModule.hpp"
#include "engine/core/entities/TestComponent.hpp"

int main() {
    parteeengine::Engine engine;

    engine.createModule<parteeengine::TestModule>();

    parteeengine::Entity entity = engine.createEntity();
    engine.addComponent<parteeengine::TestComponent>(entity);

    engine.run();

    return 0;
}