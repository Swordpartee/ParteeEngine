#include "engine/core/Engine.hpp"
#include "engine/core/modules/TestModule.hpp"
#include "engine/core/entities/TestComponent.hpp"
#include "engine/rendering/core/RenderModule2d.hpp"
#include "engine/rendering/core/RenderComponent.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"

int main() {
    parteeengine::Engine engine;

    // engine.createModule<parteeengine::TestModule>();
    engine.createModule<parteeengine::rendering::RenderModule2d>()
        .config<parteeengine::rendering::OpenGLRenderer>({800, 600, "ParteeEngine"});

    parteeengine::Entity entity = engine.createEntity();
    // engine.addComponent<parteeengine::TestComponent>(entity);
    engine.addComponent<parteeengine::TransformComponent2d>(entity) = 
        {200.f, 200.f};
    engine.addComponent<parteeengine::rendering::QuadRenderComponent>(entity) = 
        {100.f, 100.f, {255, 0, 0}};

    engine.run();

    return 0;
}