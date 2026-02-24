#pragma once

#include "Lexer.hpp"

#include <string>
#include <vector>
#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[91m" // Bright red for errors
#define GREEN   "\033[92m" // Bright green for success
#define YELLOW  "\033[93m" // Bright yellow for warnings
#define PURPLE    "\033[94m" // Bright PURPLE for information
#define MAGENTA "\033[95m" // Bright magenta for highlights
#define BLUE    "\033[96m" // Bright BLUE for neutral elements
#define GREY    "\033[37m" // Standard grey for less important text

namespace ParteeEngine {

    // AST (Abstract Syntax Tree): a simplified tree representing program structure.
    // We keep semantic nodes (likecan expressions) and omit raw punctuation.
    struct ASTNode { virtual ~ASTNode() = default; };

    // Expr (Expression): base node for any evaluatable expression.
    struct Expr : ASTNode { 
        virtual ~Expr() = default;
    };

    // Program: top-level node holding a list of expressions/statements
    struct Program : ASTNode {
        std::vector<Expr *> statements;
        ~Program() {
            for (auto *stmt : statements)
                delete stmt;
        }
    };

    struct ErrorExpr : Expr {
        std::string message;
        int row, column;
        ErrorExpr(const std::string& msg) : message(msg), row(-1), column(-1) {}
        ErrorExpr(const std::string& msg, int r, int c) : message(msg), row(r), column(c) {}
        ~ErrorExpr() = default;
    };

    // NumberLiteral: numeric constant; we store the original lexeme string.
    struct NumberLiteral : Expr {
        NumberLiteral(const std::string& val) : value(val) {}
        std::string value;
        ~NumberLiteral() = default;
    };

    struct StringLiteral : Expr {
        StringLiteral(const std::string& val) : value(val) {}
        std::string value;
        ~StringLiteral() = default;
    };

    struct BooleanLiteral : Expr {
        BooleanLiteral(bool val) : value(val) {}
        bool value;
        ~BooleanLiteral() = default;
    };

    // IdentifierExpr: reference to a named variable/function/etc.
    struct Identifier : Expr {
        Identifier(const std::string& n) : name(n) {}
        std::string name; // identifier text
    
        ~Identifier() = default;
    };

    struct ArrayLiteral : Expr {
        std::vector<Expr*> elements; // element expressions
        ~ArrayLiteral() {
            for (auto* elem : elements)
                delete elem;
        }
    };

    struct EventLiteral : Expr {
        Expr* eventName; // name of the event
        std::vector<Expr*> parameters;
        ~EventLiteral() {
            for (auto* param : parameters)
                delete param;
        }
    };  

    struct UnaryExpr : Expr {
        UnaryExpr(TokenType opType, Expr* expr)
            : operatorType(opType), operand(expr) {}
        TokenType operatorType; // operator kind
        Expr* operand{nullptr};
        ~UnaryExpr() {
            delete operand;
        }
    };

    // BinaryExpr: infix binary operator (e.g., a + b, x ** y).
    // 'op' holds the operator TokenType from the lexer.
    struct BinaryExpr : Expr {
        BinaryExpr(TokenType opType, Expr* leftExpr, Expr* rightExpr)
            : operatorType(opType), leftExpression(leftExpr), rightExpression(rightExpr) {}
        TokenType operatorType; // operator kind
        Expr* leftExpression{nullptr};
        Expr* rightExpression{nullptr};
        ~BinaryExpr() { delete leftExpression; delete rightExpression; }
    };

    struct BodyExpr : Expr {
        std::vector<Expr *> statements; // statements in the body
        ~BodyExpr() {
            for (auto *stmt : statements)
                delete stmt;
        }
    };

    struct UseExpr : Expr {
        UseExpr(Expr* modName) : moduleName(modName) {}
        Expr* moduleName; // name of the module to use
        ~UseExpr() = default;
    };

    struct OnExpr : Expr {
        Expr* eventLiteral{nullptr}; // event to listen to
        Expr* body{nullptr};         // body to execute on event
        ~OnExpr() {
            delete eventLiteral;
            delete body;
        }
    };

    struct ReturnExpr : Expr {
        ReturnExpr(Expr* val) : value(val) {}
        Expr* value{nullptr}; // return value expression
        ~ReturnExpr() {
            delete value;
        }
    };

    struct ContinueExpr : Expr {
        ~ContinueExpr() = default;
    };

    struct BreakExpr : Expr {
        ~BreakExpr() = default;
    };

    struct CaseExpr : Expr {
        Expr* caseValue{nullptr}; // value for the case
        Expr* body{nullptr};      // body to execute for this case
        ~CaseExpr() {
            delete caseValue;
            delete body;
        }
    };

    struct DefaultExpr : Expr {
        Expr* body{nullptr}; // body to execute for default case
        ~DefaultExpr() {
            delete body;
        }
    };

    struct SwitchExpr : Expr {
        Expr* expression{nullptr};               // expression to switch on
        std::vector<struct CaseExpr*> cases;     // list of case expressions
        struct DefaultExpr* defaultCase{nullptr}; // optional default case
        ~SwitchExpr() {
            delete expression;
            for (auto* caseExpr : cases)
                delete caseExpr;
            delete defaultCase;
        }
    };

    struct IfExpr : Expr {
        Expr* condition{nullptr};      // condition expression
        Expr* thenBranch{nullptr};     // 'then' branch expression
        Expr* elseBranch{nullptr};     // optional 'else' branch expression
        ~IfExpr() {
            delete condition;
            delete thenBranch;
            delete elseBranch;
        }
    };

    struct WhileExpr : Expr {
        Expr* condition{nullptr};  // loop condition expression
        Expr* body{nullptr};       // loop body expression
        ~WhileExpr() {
            delete condition;
            delete body;
        }
    };

    struct ForExpr : Expr {
        Expr* iteratorName; // loop variable name
        Expr* iterable{nullptr};  // iterable expression
        Expr* body{nullptr};      // loop body expression
        ~ForExpr() {
            delete iterable;
            delete body;
        }
    };

    struct FunctionExpr : Expr {
        Expr* name;                   // function name
        std::vector<Expr*> parameters; // parameter names
        Expr* body;                // function body expression
        ~FunctionExpr() {
            delete body;
        }
    };

    struct CallExpr : Expr {
        CallExpr(Expr* calleeExpr, const std::vector<Expr*>& args)
            : callee(calleeExpr), arguments(args) {}
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

    inline int getInfixOperatorBindingPower(TokenType type, int &rightBP) {
        switch (type) {
        case TokenType::OP_Dot: // Member access
            rightBP = 270;
            return 260;

        case TokenType::OP_Power: // Exponentiation (right-associative)
            rightBP = 170;
            return 160;

        case TokenType::KW_Not: // Logical NOT (unary, but included here for precedence)
        case TokenType::OP_Bitwise_Not:
            rightBP = 0;
            return 150;

        // Multiplicative
        case TokenType::OP_Star:
        case TokenType::OP_Slash:
        case TokenType::OP_Percent:
            rightBP = 150;
            return 140;

        // Additive
        case TokenType::OP_Plus:
        case TokenType::OP_Minus:
            rightBP = 130;
            return 120;

        // Bitwise Shifts
        case TokenType::OP_Left_Shift:
        case TokenType::OP_Right_Shift:
            rightBP = 120;
            return 110;

        // Comparison
        case TokenType::OP_Less:
        case TokenType::OP_Less_Equal:
        case TokenType::OP_Greater:
        case TokenType::OP_Greater_Equal:
        case TokenType::OP_Equal:
        case TokenType::OP_Not_Equal:
            rightBP = 110;
            return 100;

        // Bitwise AND
        case TokenType::OP_Bitwise_And:
            rightBP = 100;
            return 90;

        // Bitwise XOR
        case TokenType::OP_Bitwise_Xor:
            rightBP = 90;
            return 80;

        // Bitwise OR
        case TokenType::OP_Bitwise_Or:
            rightBP = 80;
            return 70;

        // Logical AND/OR
        case TokenType::KW_And:
            rightBP = 70;
            return 60;
        case TokenType::KW_Or:
            rightBP = 60;
            return 50;

        // Assignment (right-associative)
        case TokenType::OP_Assign:
        case TokenType::OP_Plus_Assign:
        case TokenType::OP_Minus_Assign:
        case TokenType::OP_Percent_Assign:
            rightBP = 41;
            return 40;

        default:
            return 0; // not a recognized infix operator
        }
    }

    inline void printAST(const ASTNode* node, int indent = 0) {
        if (!node) return;
        std::string indentStr(indent, ' ');
    
        if (const auto* program = dynamic_cast<const Program*>(node)) {
            std::cout << PURPLE << indentStr << "Program:" << RESET << "\n";
            for (const auto* stmt : program->statements) {
                printAST(stmt, indent);
            }
        } else if (const auto* errorNode = dynamic_cast<const ErrorExpr*>(node)) {
            std::cout << RED << indentStr << "ErrorExpr: " << GREY << errorNode->message;
            if (errorNode->row >= 0 && errorNode->column >= 0) {
                std::cout << " (at " << errorNode->row << ":" << errorNode->column << ")";
            }
            std::cout << RESET << "\n";
        } else if (const auto* numLit = dynamic_cast<const NumberLiteral*>(node)) {
            std::cout << BLUE << indentStr << "NumberLiteral: " << GREY << numLit->value << "\n";
        } else if (const auto* idExpr = dynamic_cast<const Identifier*>(node)) {
            std::cout << YELLOW << indentStr << "IdentifierExpr: " << GREY << idExpr->name << "\n";
        } else if (const auto* unaryExpr = dynamic_cast<const UnaryExpr*>(node)) {
            std::cout << MAGENTA << indentStr << "UnaryExpr: " << GREY << tokenTypeToString(unaryExpr->operatorType) << "\n";
            std::cout << BLUE << indentStr << " Operand:\n";
            printAST(unaryExpr->operand, indent + 2);
        } else if (const auto* binExpr = dynamic_cast<const BinaryExpr*>(node)) {
            std::cout << MAGENTA << indentStr << "BinaryExpr: " << GREY << tokenTypeToString(binExpr->operatorType) << "\n";
            std::cout << BLUE << indentStr << " Left:\n";
            printAST(binExpr->leftExpression, indent + 2);
            std::cout << BLUE << indentStr << " Right:\n";
            printAST(binExpr->rightExpression, indent + 2);
        } else if (const auto* callExpr = dynamic_cast<const CallExpr*>(node)) {
            std::cout << GREEN << indentStr << "CallExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Callee:\n";
            printAST(callExpr->callee, indent + 2);
            std::cout << BLUE << indentStr << " Arguments:\n";
            if (!callExpr->arguments.empty()) {
                for (const auto *arg : callExpr->arguments) {
                    printAST(arg, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (none)" << RESET << "\n";
            }
        } else if (const auto* indexExpr = dynamic_cast<const IndexExpr*>(node)) {
            std::cout << PURPLE << indentStr << "IndexExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Base:\n";
            printAST(indexExpr->base, indent + 2);
            std::cout << BLUE << indentStr << " Index:\n";
            printAST(indexExpr->index, indent + 2);
        } else if (const auto* arrayLit = dynamic_cast<const ArrayLiteral*>(node)) {
            std::cout << BLUE << indentStr << "ArrayLiteral:" << RESET;
            if (!arrayLit->elements.empty()) {
                std::cout << "\n" << BLUE << indentStr << " Elements:\n";
                for (const auto *elem : arrayLit->elements) {
                    printAST(elem, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (empty)" << RESET << "\n";
            }
        } else if (const auto* eventLit = dynamic_cast<const EventLiteral*>(node)) {
            std::cout << MAGENTA << indentStr << "EventLiteral:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Event Name:\n";
            printAST(eventLit->eventName, indent + 2);
            std::cout << BLUE << indentStr << " Parameters:\n";
            if (!eventLit->parameters.empty()) {
                for (const auto *param : eventLit->parameters) {
                    printAST(param, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (none)" << RESET << "\n";
            }
        } else if (const auto* bodyExpr = dynamic_cast<const BodyExpr*>(node)) {
            std::cout << GREEN << indentStr << "BodyExpr:" << RESET << "\n";
            if (!bodyExpr->statements.empty()) {
                for (const auto *stmt : bodyExpr->statements) {
                    printAST(stmt, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (empty)" << RESET << "\n";
            }
        } else if (const auto* strLit = dynamic_cast<const StringLiteral*>(node)) {
            std::cout << YELLOW << indentStr << "StringLiteral: " << GREY << "\"" << strLit->value << "\"" << RESET << "\n";
        } else if (const auto* boolLit = dynamic_cast<const BooleanLiteral*>(node)) {
            std::cout << YELLOW << indentStr << "BooleanLiteral: " << RESET << BLUE << (boolLit->value ? "true" : "false") << RESET << "\n";
        } else if (const auto* useExpr = dynamic_cast<const UseExpr*>(node)) {
            std::cout << GREEN << indentStr << "UseExpr:" << RESET << "\n";
            std::cout << PURPLE << indentStr << " Module Name:" << RESET << "\n";
            printAST(useExpr->moduleName, indent + 2);
        } else if (const auto* onExpr = dynamic_cast<const OnExpr*>(node)) {
            std::cout << GREEN << indentStr << "OnExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Event:" << RESET << "\n";
            printAST(onExpr->eventLiteral, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(onExpr->body, indent + 2);
        } else if (const auto* returnExpr = dynamic_cast<const ReturnExpr*>(node)) {
            std::cout << GREEN << indentStr << "ReturnExpr:" << RESET << "\n";
            if (returnExpr->value) {
                std::cout << MAGENTA << indentStr << " Value:" << RESET << "\n";
                printAST(returnExpr->value, indent + 2);
            } else {
                std::cout << MAGENTA << indentStr << " (no value)" << RESET << "\n";
            }
        } else if (const auto* continueExpr = dynamic_cast<const ContinueExpr*>(node)) {
            std::cout << GREEN << indentStr << "ContinueExpr" << RESET << "\n";
        } else if (const auto* breakExpr = dynamic_cast<const BreakExpr*>(node)) {
            std::cout << GREEN << indentStr << "BreakExpr" << RESET << "\n";
        } else if (const auto* ifExpr = dynamic_cast<const IfExpr*>(node)) {
            std::cout << PURPLE << indentStr << "IfExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Condition:" << RESET << "\n";
            printAST(ifExpr->condition, indent + 2);
            std::cout << MAGENTA << indentStr << " Then Branch:" << RESET << "\n";
            printAST(ifExpr->thenBranch, indent + 2);
            if (ifExpr->elseBranch) {
                std::cout << MAGENTA << indentStr << " Else Branch:" << RESET << "\n";
                printAST(ifExpr->elseBranch, indent + 2);
            }
        } else if (const auto* whileExpr = dynamic_cast<const WhileExpr*>(node)) {
            std::cout << PURPLE << indentStr << "WhileExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Condition:" << RESET << "\n";
            printAST(whileExpr->condition, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(whileExpr->body, indent + 2);
        } else if (const auto* forExpr = dynamic_cast<const ForExpr*>(node)) {
            std::cout << PURPLE << indentStr << "ForExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Iterator Name:" << RESET << "\n";
            printAST(forExpr->iteratorName, indent + 2);
            std::cout << MAGENTA << indentStr << " Iterable:" << RESET << "\n";
            printAST(forExpr->iterable, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(forExpr->body, indent + 2);
        } else if (const auto* funcExpr = dynamic_cast<const FunctionExpr*>(node)) {
            std::cout << GREEN << indentStr << "FunctionExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Name:" << RESET << "\n";
            printAST(funcExpr->name, indent + 2);
            std::cout << MAGENTA << indentStr << " Parameters:" << RESET << "\n";
            if (!funcExpr->parameters.empty()) {
                for (const auto *param : funcExpr->parameters) {
                    printAST(param, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (none)" << RESET << "\n";
            }
            std::cout << BLUE << indentStr << " Body:\n";
            printAST(funcExpr->body, indent + 2);
        } else {
            std::cout << RED << indentStr << "Unknown ASTNode type\n";
        }
    }

} // namespace ParteeEngine
