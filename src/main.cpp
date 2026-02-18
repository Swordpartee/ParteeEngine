#include "engine/core/Engine.hpp"

#include "engine/rendering/modules/RenderModule2d.hpp"
#include "engine/rendering/renderers/OpenGLRenderer.hpp"
#include "engine/physics/PhysicsModule.hpp"

#include "engine/rendering/components/RenderComponent2d.hpp"
#include "engine/rendering/core/RenderCommand.hpp"

#include "interpreter/Lexer.hpp"
#include "interpreter/Parser.hpp"
#include "interpreter/ScriptLoader.hpp"
#include "interpreter/AST.hpp"

#include <filesystem>
#include <iostream>

bool runEngine() {
    using namespace ParteeEngine;

    Engine engine;

    engine.addModule<Rendering::RenderModule2d<Rendering::OpenGLRenderer>>();
    engine.addModule<PhysicsModule>();

    Entity &entity = engine.createEntity().with<Rendering::RenderComponent2d<Rendering::DrawQuadCommand>>([](auto &comp) {
        comp.getCommand() = Rendering::DrawQuadCommand(
            Rendering::Quad{
                .x = 100.f,
                .y = 100.f,
                .width = 200.f,
                .height = 150.f,
                .color = {255, 0, 0, 255}
            }
        );
    });

    engine.run();

    return 0;
}

bool lexerTest() {
    using namespace ParteeEngine;

    Lexer lexer(ScriptLoader::loadScript("exampleCode.par"));

    auto tokens = lexer.tokenize();

    // for (const auto &token : tokens) {
    //     std::cout << "Token: " << tokenTypeToString(token.type)
    //               << " Value: \"" << token.value << "\""
    //               << " Line: " << token.line
    //               << " Column: " << token.column << "\n";
    // }

    Parser parser;

    ParseResult parseResult = parser.parse(tokens);

    if (parseResult.ok)
        printAST(parseResult.expr);
    
    return 0;

}
    

int main() {
    return runEngine();
    // return lexerTest();
}