#include "engine/Engine.hpp"
#include "engine/modules/RenderModule2d.hpp"
#include "engine/modules/PhysicsModule.hpp"
#include "engine/entities/components/TransformComponent.hpp"
#include "engine/entities/components/RigidBodyComponent.hpp"
#include "engine/entities/components/BoxColliderComponent.hpp"
#include "engine/RenderableFactory.hpp"

#include "interpreter/Lexer.hpp"
#include "interpreter/Parser.hpp"
#include "interpreter/ScriptLoader.hpp"
#include "interpreter/AST.hpp"

#include <filesystem>
#include <iostream>

boolean runEngine() {
    using namespace ParteeEngine;
    Engine engine;

    engine.addModule<RenderModule2d, int, int>(800, 600);
    engine.addModule<PhysicsModule>();

    Entity& entity = RenderableFactory::createSquare(engine);
    entity.addComponent<RigidBodyComponent>();
    entity.addComponent<BoxColliderComponent>();

    entity.getComponent<TransformComponent>()->setPosition({200.0f, 150.0f, 0.0f});
    entity.getComponent<TransformComponent>()->setScale({50.0f, 50.0f, 5.0f});

    entity.getComponent<RigidBodyComponent>()->setVelocity({10.0f, 0.0f, 0.0f});

    Entity& entity2 = RenderableFactory::createSquare(engine);
    entity2.addComponent<BoxColliderComponent>();

    entity2.getComponent<TransformComponent>()->setPosition({400.0f, 500.0f, 0.0f});
    entity2.getComponent<TransformComponent>()->setScale({800.0f, 20.0f, 5.0f});

    Entity &entity3 = RenderableFactory::createSquare(engine);
    entity3.addComponent<RigidBodyComponent>();
    entity3.addComponent<BoxColliderComponent>();

    entity3.getComponent<TransformComponent>()->setPosition({500.0f, 150.0f, 0.0f});
    entity3.getComponent<TransformComponent>()->setScale({50.0f, 50.0f, 5.0f});

    entity3.getComponent<RigidBodyComponent>()->setVelocity({-20.0f, 0.0f, 0.0f});

    engine.run();

    return 0;
}

boolean lexerTest() {
    using namespace ParteeEngine;

    Lexer lexer(ScriptLoader::loadScript("exampleCode.par"));

    auto tokens = lexer.tokenize();

    for (const auto &token : tokens) {
        std::cout << "Token: " << tokenTypeToString(token.type)
                  << " Value: \"" << token.value << "\""
                  << " Line: " << token.line
                  << " Column: " << token.column << "\n";
    }

    Parser parser;

    ParseResult parseResult = parser.parseExpression(tokens);

    printAST(parseResult.expr);
    
    return 0;

}
    

int main() {
    // return runEngine();
    return lexerTest();
}