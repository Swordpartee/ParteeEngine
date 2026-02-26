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

namespace interpreter {

    enum class ExprType {
        Program,
        Error,
        NumberLiteral,
        StringLiteral,
        BooleanLiteral,
        Identifier,
        ArrayLiteral,
        EventLiteral,
        UnaryExpr,
        BinaryExpr,
        BodyExpr,
        UseExpr,
        OnExpr,
        ReturnExpr,
        ContinueExpr,
        BreakExpr,
        CaseExpr,
        DefaultExpr,
        SwitchExpr,
        IfExpr,
        WhileExpr,
        ForExpr,
        FunctionExpr,
        CallExpr,
        IndexExpr
    };

    // AST (Abstract Syntax Tree): a simplified tree representing program structure.
    // We keep semantic nodes (likecan expressions) and omit raw punctuation.
    struct ASTNode { 
        virtual ~ASTNode() = default;
        virtual ExprType getType() const = 0;
    };

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
        ExprType getType() const override { return ExprType::Program; }
    };

    struct ErrorExpr : Expr {
        std::string message;
        int row, column;
        ErrorExpr(const std::string& msg) : message(msg), row(-1), column(-1) {}
        ErrorExpr(const std::string& msg, int r, int c) : message(msg), row(r), column(c) {}
        ~ErrorExpr() = default;
        ExprType getType() const override { return ExprType::Error; }
    };

    // NumberLiteral: numeric constant; we store the original lexeme string.
    struct NumberLiteral : Expr {
        NumberLiteral(const std::string& val) : value(std::stod(val)) {}
        double value;
        ~NumberLiteral() = default;
        ExprType getType() const override { return ExprType::NumberLiteral; }
    };

    struct StringLiteral : Expr {
        StringLiteral(const std::string& val) : value(val) {}
        std::string value;
        ~StringLiteral() = default;
        ExprType getType() const override { return ExprType::StringLiteral; }
    };

    struct BooleanLiteral : Expr {
        BooleanLiteral(bool val) : value(val) {}
        bool value;
        ~BooleanLiteral() = default;
        ExprType getType() const override { return ExprType::BooleanLiteral; }
    };

    // IdentifierExpr: reference to a named variable/function/etc.
    struct Identifier : Expr {
        Identifier(const std::string& n) : name(n) {}
        std::string name; // identifier text
    
        ~Identifier() = default;
        ExprType getType() const override { return ExprType::Identifier; }
    };

    struct ArrayLiteral : Expr {
        std::vector<Expr*> elements; // element expressions
        ~ArrayLiteral() {
            for (auto* elem : elements)
                delete elem;
        }
        ExprType getType() const override { return ExprType::ArrayLiteral; }
    };

    struct EventLiteral : Expr {
        Expr* eventName; // name of the event
        std::vector<Expr*> parameters;
        ~EventLiteral() {
            for (auto* param : parameters)
                delete param;
        }
        ExprType getType() const override { return ExprType::EventLiteral; }
    };  

    struct UnaryExpr : Expr {
        UnaryExpr(TokenType opType, Expr* expr)
            : operatorType(opType), operand(expr) {}
        TokenType operatorType; // operator kind
        Expr* operand{nullptr};
        ~UnaryExpr() {
            delete operand;
        }
        ExprType getType() const override { return ExprType::UnaryExpr; }
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
        ExprType getType() const override { return ExprType::BinaryExpr; }
    };

    struct BodyExpr : Expr {
        std::vector<Expr *> statements; // statements in the body
        ~BodyExpr() {
            for (auto *stmt : statements)
                delete stmt;
        }
        ExprType getType() const override { return ExprType::BodyExpr; }
    };

    struct UseExpr : Expr {
        UseExpr(Expr* modName) : moduleName(modName) {}
        Expr* moduleName; // name of the module to use
        ~UseExpr() = default;
        ExprType getType() const override { return ExprType::UseExpr; }
    };

    struct OnExpr : Expr {
        Expr* eventLiteral{nullptr}; // event to listen to
        Expr* body{nullptr};         // body to execute on event
        ~OnExpr() {
            delete eventLiteral;
            delete body;
        }
        ExprType getType() const override { return ExprType::OnExpr; }
    };

    struct ReturnExpr : Expr {
        ReturnExpr(Expr* val) : value(val) {}
        Expr* value{nullptr}; // return value expression
        ~ReturnExpr() {
            delete value;
        }
        ExprType getType() const override { return ExprType::ReturnExpr; }
    };

    struct ContinueExpr : Expr {
        ~ContinueExpr() = default;
        ExprType getType() const override { return ExprType::ContinueExpr; }
    };

    struct BreakExpr : Expr {
        ~BreakExpr() = default;
        ExprType getType() const override { return ExprType::BreakExpr; }
    };

    struct CaseExpr : Expr {
        Expr* caseValue{nullptr}; // value for the case
        Expr* body{nullptr};      // body to execute for this case
        ~CaseExpr() {
            delete caseValue;
            delete body;
        }
        ExprType getType() const override { return ExprType::CaseExpr; }
    };

    struct DefaultExpr : Expr {
        Expr* body{nullptr}; // body to execute for default case
        ~DefaultExpr() {
            delete body;
        }
        ExprType getType() const override { return ExprType::DefaultExpr; }
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
        ExprType getType() const override { return ExprType::SwitchExpr; }
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
        ExprType getType() const override { return ExprType::IfExpr; }
    };

    struct WhileExpr : Expr {
        Expr* condition{nullptr};  // loop condition expression
        Expr* body{nullptr};       // loop body expression
        ~WhileExpr() {
            delete condition;
            delete body;
        }
        ExprType getType() const override { return ExprType::WhileExpr; }
    };

    struct ForExpr : Expr {
        Expr* iteratorName; // loop variable name
        Expr* iterable{nullptr};  // iterable expression
        Expr* body{nullptr};      // loop body expression
        ~ForExpr() {
            delete iterable;
            delete body;
        }
        ExprType getType() const override { return ExprType::ForExpr; }
    };

    struct FunctionExpr : Expr {
        Expr* name;                   // function name
        std::vector<Expr*> parameters; // parameter names
        Expr* body;                // function body expression
        ~FunctionExpr() {
            delete body;
        }
        ExprType getType() const override { return ExprType::FunctionExpr; }
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
        ExprType getType() const override { return ExprType::CallExpr; }
    };


    struct IndexExpr : Expr {
        Expr *base{nullptr};  // base expression being indexed
        Expr *index{nullptr}; // index expression
        ~IndexExpr() {
            delete base;
            delete index;
        }
        ExprType getType() const override { return ExprType::IndexExpr; }
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
        ExprType type = node->getType();

        switch (type) {
        case ExprType::Program: {
            auto* program = static_cast<const Program*>(node);
            std::cout << PURPLE << indentStr << "Program:" << RESET << "\n";
            for (const auto* stmt : program->statements) {
                printAST(stmt, indent);
            }
            break;
        }
        case ExprType::Error: {
            auto* errorNode = static_cast<const ErrorExpr*>(node);
            std::cout << RED << indentStr << "ErrorExpr: " << GREY << errorNode->message;
            if (errorNode->row >= 0 && errorNode->column >= 0) {
                std::cout << " (at " << errorNode->row << ":" << errorNode->column << ")";
            }
            std::cout << RESET << "\n";
            break;
        }
        case ExprType::NumberLiteral: {
            auto* numLit = static_cast<const NumberLiteral*>(node);
            std::cout << BLUE << indentStr << "NumberLiteral: " << GREY << numLit->value << "\n";
            break;
        }
        case ExprType::Identifier: {
            auto* idExpr = static_cast<const Identifier*>(node);
            std::cout << YELLOW << indentStr << "IdentifierExpr: " << GREY << idExpr->name << "\n";
            break;
        }
        case ExprType::UnaryExpr: {
            auto* unaryExpr = static_cast<const UnaryExpr*>(node);
            std::cout << MAGENTA << indentStr << "UnaryExpr: " << GREY << tokenTypeToString(unaryExpr->operatorType) << "\n";
            std::cout << BLUE << indentStr << " Operand:\n";
            printAST(unaryExpr->operand, indent + 2);
            break;
        }
        case ExprType::BinaryExpr: {
            auto* binExpr = static_cast<const BinaryExpr*>(node);
            std::cout << MAGENTA << indentStr << "BinaryExpr: " << GREY << tokenTypeToString(binExpr->operatorType) << "\n";
            std::cout << BLUE << indentStr << " Left:\n";
            printAST(binExpr->leftExpression, indent + 2);
            std::cout << BLUE << indentStr << " Right:\n";
            printAST(binExpr->rightExpression, indent + 2);
            break;
        }
        case ExprType::CallExpr: {
            auto* callExpr = static_cast<const CallExpr*>(node);
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
            break;
        }
        case ExprType::IndexExpr: {
            auto* indexExpr = static_cast<const IndexExpr*>(node);
            std::cout << PURPLE << indentStr << "IndexExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Base:\n";
            printAST(indexExpr->base, indent + 2);
            std::cout << BLUE << indentStr << " Index:\n";
            printAST(indexExpr->index, indent + 2);
            break;
        }
        case ExprType::ArrayLiteral: {
            auto* arrayLit = static_cast<const ArrayLiteral*>(node);
            std::cout << BLUE << indentStr << "ArrayLiteral:" << RESET;
            if (!arrayLit->elements.empty()) {
                std::cout << "\n" << BLUE << indentStr << " Elements:\n";
                for (const auto *elem : arrayLit->elements) {
                    printAST(elem, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (empty)" << RESET << "\n";
            }
            break;
        }
        case ExprType::EventLiteral: {
            auto* eventLit = static_cast<const EventLiteral*>(node);
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
            break;
        }
        case ExprType::BodyExpr: {
            auto* bodyExpr = static_cast<const BodyExpr*>(node);
            std::cout << GREEN << indentStr << "BodyExpr:" << RESET << "\n";
            if (!bodyExpr->statements.empty()) {
                for (const auto *stmt : bodyExpr->statements) {
                    printAST(stmt, indent + 2);
                }
            } else {
                std::cout << GREY << indentStr << " (empty)" << RESET << "\n";
            }
            break;
        }
        case ExprType::StringLiteral: {
            auto* strLit = static_cast<const StringLiteral*>(node);
            std::cout << YELLOW << indentStr << "StringLiteral: " << GREY << "\"" << strLit->value << "\"" << RESET << "\n";
            break;
        }
        case ExprType::BooleanLiteral: {
            auto* boolLit = static_cast<const BooleanLiteral*>(node);
            std::cout << YELLOW << indentStr << "BooleanLiteral: " << RESET << BLUE << (boolLit->value ? "true" : "false") << RESET << "\n";
            break;
        }
        case ExprType::UseExpr: {
            auto* useExpr = static_cast<const UseExpr*>(node);
            std::cout << GREEN << indentStr << "UseExpr:" << RESET << "\n";
            std::cout << PURPLE << indentStr << " Module Name:" << RESET << "\n";
            printAST(useExpr->moduleName, indent + 2);
            break;
        }
        case ExprType::OnExpr: {
            auto* onExpr = static_cast<const OnExpr*>(node);
            std::cout << GREEN << indentStr << "OnExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Event:" << RESET << "\n";
            printAST(onExpr->eventLiteral, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(onExpr->body, indent + 2);
            break;
        }
        case ExprType::ReturnExpr: {
            auto* returnExpr = static_cast<const ReturnExpr*>(node);
            std::cout << GREEN << indentStr << "ReturnExpr:" << RESET << "\n";
            if (returnExpr->value) {
                std::cout << MAGENTA << indentStr << " Value:" << RESET << "\n";
                printAST(returnExpr->value, indent + 2);
            } else {
                std::cout << MAGENTA << indentStr << " (no value)" << RESET << "\n";
            }
            break;
        }
        case ExprType::ContinueExpr:
            std::cout << GREEN << indentStr << "ContinueExpr" << RESET << "\n";
            break;
        case ExprType::BreakExpr:
            std::cout << GREEN << indentStr << "BreakExpr" << RESET << "\n";
            break;
        case ExprType::IfExpr: {
            auto* ifExpr = static_cast<const IfExpr*>(node);
            std::cout << PURPLE << indentStr << "IfExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Condition:" << RESET << "\n";
            printAST(ifExpr->condition, indent + 2);
            std::cout << MAGENTA << indentStr << " Then Branch:" << RESET << "\n";
            printAST(ifExpr->thenBranch, indent + 2);
            if (ifExpr->elseBranch) {
                std::cout << MAGENTA << indentStr << " Else Branch:" << RESET << "\n";
                printAST(ifExpr->elseBranch, indent + 2);
            }
            break;
        }
        case ExprType::WhileExpr: {
            auto* whileExpr = static_cast<const WhileExpr*>(node);
            std::cout << PURPLE << indentStr << "WhileExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Condition:" << RESET << "\n";
            printAST(whileExpr->condition, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(whileExpr->body, indent + 2);
            break;
        }
        case ExprType::ForExpr: {
            auto* forExpr = static_cast<const ForExpr*>(node);
            std::cout << PURPLE << indentStr << "ForExpr:" << RESET << "\n";
            std::cout << MAGENTA << indentStr << " Iterator Name:" << RESET << "\n";
            printAST(forExpr->iteratorName, indent + 2);
            std::cout << MAGENTA << indentStr << " Iterable:" << RESET << "\n";
            printAST(forExpr->iterable, indent + 2);
            std::cout << MAGENTA << indentStr << " Body:" << RESET << "\n";
            printAST(forExpr->body, indent + 2);
            break;
        }
        case ExprType::FunctionExpr: {
            auto* funcExpr = static_cast<const FunctionExpr*>(node);
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
            break;
        }
        case ExprType::CaseExpr: {
            auto* caseExpr = static_cast<const CaseExpr*>(node);
            std::cout << MAGENTA << indentStr << "CaseExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Value:" << RESET << "\n";
            printAST(caseExpr->caseValue, indent + 2);
            std::cout << BLUE << indentStr << " Body:" << RESET << "\n";
            printAST(caseExpr->body, indent + 2);
            break;
        }
        case ExprType::DefaultExpr: {
            auto* defaultExpr = static_cast<const DefaultExpr*>(node);
            std::cout << MAGENTA << indentStr << "DefaultExpr:" << RESET << "\n";
            if (defaultExpr->body) {
                std::cout << BLUE << indentStr << " Body:" << RESET << "\n";
                printAST(defaultExpr->body, indent + 2);
            }
            break;
        }
        case ExprType::SwitchExpr: {
            auto* switchExpr = static_cast<const SwitchExpr*>(node);
            std::cout << PURPLE << indentStr << "SwitchExpr:" << RESET << "\n";
            std::cout << BLUE << indentStr << " Expression:" << RESET << "\n";
            printAST(switchExpr->expression, indent + 2);
            std::cout << BLUE << indentStr << " Cases:" << RESET << "\n";
            for (const auto* caseExpr : switchExpr->cases) {
                printAST(caseExpr, indent + 2);
            }
            if (switchExpr->defaultCase) {
                std::cout << BLUE << indentStr << " Default:" << RESET << "\n";
                printAST(switchExpr->defaultCase, indent + 2);
            }
            break;
        }
        }
    }

} // namespace ParteeEngine
