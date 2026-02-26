#pragma once

#include <vector>
#include <string>
#include <stack>

namespace interpreter {

    // Forward declarations of AST (Abstract Syntax Tree) node types
    struct ASTNode;
    struct Program;            // top-level program node
    struct Expr;               // Expression base node
    struct NumberLiteral;      // Numeric literal expression
    struct Identifier;     // Identifier expression
    struct BinaryExpr;         // Binary operator expression
    struct CallExpr;           // Function call expression
    struct IndexExpr;          // Indexing expression
    struct BodyExpr;           // Block/body expression

    // ParseResult: outcome wrapper for parsing operations.
    // 'expr' is the resulting AST expression (or nullptr on failure).
    // 'ok' indicates success; 'message' carries a short error description on failure.
    struct ParseResult {
        Program* program;
        bool ok;
        const char* message;
    };

    enum class ParsingContext {
        Global,
        Function,
        Loop,
        Conditional,
        Switch
    };

    struct Token; // forward declaration (see Lexer.hpp)
    enum class TokenType; // forward declaration for operator/terminal kinds

    class Parser {
    public:
        Parser() = default;

        // Parse a vector of tokens into an expression AST.
        // Minimal skeleton: supports numbers, identifiers, parentheses, + - * /
        ParseResult parse(const std::vector<Token>& tokens);
    
    private:
        // Cursor: lightweight iterator over the token stream.
        // Abbrev: EOF (End Of File) — special token marking end of input.
        struct Cursor {
            const std::vector<Token>* tokens;
            size_t pos{0};

            const Token& peek() const;     // look at current token without consuming
            const Token& advance();         // consume current token and move forward
            bool match(TokenType t);        // consume if current token matches t
            bool atEnd() const;             // true if at EOF or past last token
        };

        bool errorReported = false;
        std::stack<ParsingContext> contextStack;

        void enterContext(ParsingContext context);
        void exitContext();
        ParsingContext currentContext();
        bool inContext(ParsingContext context);
        
        Program* parseProgram(Cursor& cursor);

        // Pratt parser core:
        // minBP (minimum Binding Power): precedence threshold used to stop when next operator has lower precedence.
        Expr* parseExpr(Cursor& cur, int minBP);

        std::vector<Expr*> parseArguments(Cursor& cursor);

        Expr*  parseBodyExpr(Cursor& cursor);

        Expr* parseArrayLiteral(Cursor& cursor);

        Expr* parseEventLiteral(Cursor& cursor);

        Expr* parseBinaryExpr(Cursor& cursor, Expr* left, int minBP);
        
        Expr* parseCallExpr(Cursor& cursor, Expr* left);

        Expr* parseIndexExpr(Cursor& cursor, Expr* left);

        Expr* parseOnExpr(Cursor& cursor);

        Expr* parseIfExpr(Cursor& cursor);

        Expr* parseForLoop(Cursor& cursor);

        Expr* parseWhileLoop(Cursor& cursor);

        Expr* parseFunctionExpr(Cursor& cursor);

        // Synchronize method to recover from errors and prevent cascading failures.
        void synchronize(Cursor& cursor);

        Expr* reportError(const std::string& message, Cursor& cursor);

    };

}   // namespace interpreter