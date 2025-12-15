#pragma once

#include <vector>

namespace ParteeEngine {

    // Forward declarations of AST (Abstract Syntax Tree) node types
    struct ASTNode;
    struct Program;            // top-level program node
    struct Expr;               // Expression base node
    struct NumberLiteralExpr;      // Numeric literal expression
    struct IdentifierExpr;     // Identifier expression
    struct BinaryExpr;         // Binary operator expression

    // ParseResult: outcome wrapper for parsing operations.
    // 'expr' is the resulting AST expression (or nullptr on failure).
    // 'ok' indicates success; 'message' carries a short error description on failure.
    struct ParseResult {
        Expr* expr;
        bool ok;
        const char* message;
    };

    struct Token; // forward declaration (see Lexer.hpp)
    enum class TokenType; // forward declaration for operator/terminal kinds

    class Parser {
    public:
        Parser() = default;

        // Parse a vector of tokens into an expression AST.
        // Minimal skeleton: supports numbers, identifiers, parentheses, + - * /
        ParseResult parseExpression(const std::vector<Token>& tokens);
    
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

        // Pratt parser core:
        // minBP (minimum Binding Power): precedence threshold used to stop when next operator has lower precedence.
        Expr* parseExpr(Cursor& cur, int minBP);

        // parsePrimary: parses literals, identifiers, and parenthesized expressions.
        Expr* parsePrimary(Cursor& cur);

        Program* parseProgram(Cursor& cursor);
        
    };
}   // namespace ParteeEngine