#include "engine/core/Engine.hpp"

#include "engine/input/InputSystem.hpp"
#include "engine/input/devices/Keyboard.hpp"

#include "engine/core/modules/BehaviorModule.hpp"
#include "engine/rendering/core/RenderModule.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"

#include "engine/core/entities/BehaviorComponent.hpp"
#include "engine/core/entities/TransformComponent2d.hpp"
#include "engine/rendering/renderables/RenderQuad.hpp"

#include "engine/interpreter/Lexer.hpp"
#include "engine/interpreter/Parser.hpp"
#include "engine/interpreter/Interpreter.hpp"
#include "engine/interpreter/ScriptLoader.hpp"
#include "engine/interpreter/AST.hpp"

#include <iostream>
#include <random>
#include <filesystem>

using namespace parteeengine;

int engine() {
    Engine engine;

    engine.createModule<BehaviorModule>();
    engine.createModule<rendering::RenderModule<rendering::OpenGLRenderer>>()
        .registerComponent<rendering::QuadRenderCommand>(
            rendering::RenderQuadComponent::gatherer(),
            rendering::RenderQuadComponent::openGLHandler()
        );

    engine.addScript("assets/scripts/exampleCode.par");

    input::InputSystem::registerDevice<input::Keyboard>();

    // Random number generators for positions and colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDistX(0.f, 800.f);
    std::uniform_real_distribution<float> posDistY(0.f, 600.f);
    std::uniform_real_distribution<float> colorDist(0.f, 1.f);

    // Create 100 randomly placed and colored entities
    for (int i = 0; i < 1000; ++i) {
        Entity entity = engine.createEntity();
        
        // Add transform with random position
        engine.addComponent<TransformComponent2d>(entity) 
            = {posDistX(gen), posDistY(gen)};
        
        // Add render component with random color
        engine.addComponent<rendering::RenderQuadComponent>(entity)
            = {{colorDist(gen), colorDist(gen), colorDist(gen), 1.f}};
    }

    engine.run();
    
    return 0;
}

int main() {
    return engine();
}