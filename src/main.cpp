#include "engine/core/Engine.hpp"
#include "engine/core/modules/TestModule.hpp"
#include "engine/core/entities/TestComponent.hpp"
#include "engine/rendering/core/RenderModule2d.hpp"
#include "engine/rendering/core/RenderComponent.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"
#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"

using namespace parteeengine;

int main() {
    Engine engine;

    engine.createModule<rendering::RenderModule2d>()
        .config<rendering::OpenGLRenderer>({800, 600, "ParteeEngine"});

    input::InputSystem::registerDevice<input::Keyboard>();

    Entity entity = engine.createEntity();
    engine.addComponent<TransformComponent2d>(entity) 
        = {200.f, 200.f};
    engine.addComponent<rendering::QuadRenderComponent>(entity) 
        = {{255, 0, 0}};

    engine.run();

    return 0;
}