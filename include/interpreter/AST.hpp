#pragma once

#include "Lexer.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace ParteeEngine {

    // AST (Abstract Syntax Tree): a simplified tree representing program structure.
    // We keep semantic nodes (like expressions) and omit raw punctuation.
    struct ASTNode { virtual ~ASTNode() = default; };

    // Program: top-level node holding a list of expressions/statements
    struct Program : ASTNode {
        std::vector<Expr *> statements;
        ~Program() {
            for (auto *stmt : statements)
                delete stmt;
        }
    };

    // Expr (Expression): base node for any evaluatable expression.
    struct Expr : ASTNode { };

    // NumberLiteral: numeric constant; we store the original lexeme string.
    struct NumberLiteralExpr : Expr {
        std::string value; // lexeme (e.g., "42", "3.14")
    };

    struct ArrayLiteralExpr : Expr {
        std::vector<Expr*> elements; // element expressions
        ~ArrayLiteralExpr() {
            for (auto* elem : elements)
                delete elem;
        }
    };

    // IdentifierExpr: reference to a named variable/function/etc.
    struct IdentifierExpr : Expr {
        std::string name; // identifier text
    };

    // BinaryExpr: infix binary operator (e.g., a + b, x ** y).
    // 'op' holds the operator TokenType from the lexer.
    struct BinaryExpr : Expr {
        TokenType operatorType; // operator kind
        Expr* leftExpression{nullptr};
        Expr* rightExpression{nullptr};
        ~BinaryExpr() { delete leftExpression; delete rightExpression; }
    };

    inline int getInfixOperatorBindingPower(TokenType type, int& rightBP) {
        switch (type) {
            case TokenType::DEL_LParen: // Function call
                rightBP = 290; return 280;
            case TokenType::DEL_LBracket: // Indexing
                rightBP = 290; return 280;

            case TokenType::OP_Dot: // Member access
                rightBP = 270; return 260;

            case TokenType::OP_Power: // Exponentiation (right-associative)
                rightBP = 170; return 160;

            // Multiplicative
            case TokenType::OP_Star:
            case TokenType::OP_Slash:
            case TokenType::OP_Percent:
                rightBP = 150; return 140;

            // Additive
            case TokenType::OP_Plus:
            case TokenType::OP_Minus:
                rightBP = 130; return 120;

            // Comparison
            case TokenType::OP_Less:
            case TokenType::OP_Less_Equal:
            case TokenType::OP_Greater:
            case TokenType::OP_Greater_Equal:
            case TokenType::OP_Equal:
            case TokenType::OP_Not_Equal:
                rightBP = 110; return 100;

            // Logical AND/OR
            case TokenType::KW_And:
                rightBP = 90; return 80;
            case TokenType::KW_Or:
                rightBP = 70; return 60;

            // Assignment (right-associative)
            case TokenType::OP_Assign:
            case TokenType::OP_Plus_Assign:
            case TokenType::OP_Minus_Assign:
            case TokenType::OP_Percent_Assign:
                rightBP = 41; return 40;

            default:
                return 0; // not a recognized infix operator
        }
    }

    // struct if

    // struct FunctionExpr : Expr {
    //     std::string name;                   // function name
    //     std::vector<std::string> parameters; // parameter names
    //     Expr* body{nullptr};                // function body expression
    //     ~FunctionExpr() {
    //         delete body;
    //     }
    // };

    struct CallExpr : Expr {
        Expr *callee{nullptr};         // expression being called
        std::vector<Expr *> arguments; // argument expressions
        ~CallExpr() {
            delete callee;
            for (auto arg : arguments) delete arg;
        }
    };

    struct IndexExpr : Expr {
        Expr *base{nullptr};  // base expression being indexed
        Expr *index{nullptr}; // index expression
        ~IndexExpr() {
            delete base;
            delete index;
        }
    };

    inline void printAST(const ASTNode* node, int indent = 0) {
        if (!node) return;
        std::string indentStr(indent, ' ');
    
        if (const auto* program = dynamic_cast<const Program*>(node)) {
            std::cout << indentStr << "Program:\n";
            for (const auto* stmt : program->statements) {
                printAST(stmt, indent);
            }
        } else
        if (const auto* numLit = dynamic_cast<const NumberLiteralExpr*>(node)) {
            std::cout << indentStr << "NumberLiteral: " << numLit->value << "\n";
        } else if (const auto* idExpr = dynamic_cast<const IdentifierExpr*>(node)) {
            std::cout << indentStr << "IdentifierExpr: " << idExpr->name << "\n";
        } else if (const auto* binExpr = dynamic_cast<const BinaryExpr*>(node)) {
            std::cout << indentStr << "BinaryExpr: " << tokenTypeToString(binExpr->operatorType) << "\n";
            printAST(binExpr->leftExpression, indent + 2);
            printAST(binExpr->rightExpression, indent + 2);
        } else if (const auto* callExpr = dynamic_cast<const CallExpr*>(node)) {
            std::cout << indentStr << "CallExpr:\n";
            std::cout << indentStr << " Callee:\n";
            printAST(callExpr->callee, indent + 2);
            std::cout << indentStr << " Arguments:\n";
            for (const auto* arg : callExpr->arguments) {
                printAST(arg, indent + 2);
            }
        } else if (const auto* indexExpr = dynamic_cast<const IndexExpr*>(node)) {
            std::cout << indentStr << "IndexExpr:\n";
            std::cout << indentStr << " Base:\n";
            printAST(indexExpr->base, indent + 2);
            std::cout << indentStr << " Index:\n";
            printAST(indexExpr->index, indent + 2);
        } else if (const auto* arrayLit = dynamic_cast<const ArrayLiteralExpr*>(node)) {
            std::cout << indentStr << "ArrayLiteral:";
            if (!arrayLit->elements.empty()) {
                std::cout << "\n" << indentStr << " Elements:\n";
                for (const auto *elem : arrayLit->elements) {
                    printAST(elem, indent + 2);
                }
            } else {
                std::cout << " (empty)\n";
            }

        } else {
            std::cout << indentStr << "Unknown ASTNode type\n";
        }
    }

} // namespace ParteeEngine
