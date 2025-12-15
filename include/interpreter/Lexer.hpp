#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ParteeEngine {

    enum class TokenType {
        // Literals
        Identifier,
        Number,
        String,
        
        // Keywords
        KW_Use,
        KW_For,
        KW_In,
        KW_Range,
        KW_If,
        KW_Else,
        KW_Else_If,
        KW_End,
        KW_Update,
        KW_Function,
        KW_Return,
        KW_While,
        KW_Continue,
        KW_Break,
        KW_Match,
        KW_Case,
        KW_Default,
        KW_On,
        KW_And,
        KW_Or,
        KW_Not,
        KW_True,
        KW_False,
        KW_Null,
        KW_Print,
        KW_Exit,
        
        // Operators
        OP_Assign,          // =
        OP_Plus,            // +
        OP_Minus,           // -
        OP_Star,            // *
        OP_Power,           // **
        OP_Slash,           // /
        OP_Percent,         // %
        OP_Plus_Assign,     // +=
        OP_Minus_Assign,    // -=
        OP_Percent_Assign,  // %=
        OP_Increment,       // ++
        OP_Decrement,       // --
        OP_Equal,           // ==
        OP_Not_Equal,       // !=
        OP_Less,            // <
        OP_Less_Equal,      // <=
        OP_Greater,         // >
        OP_Greater_Equal,   // >=
        OP_Bitwise_And,     // &
        OP_Bitwise_Or,      // |
        OP_Bitwise_Xor,     // ^
        OP_Bitwise_Not,     // ~
        OP_Left_Shift,      // <<
        OP_Right_Shift,     // >>
        OP_Dot,             // .
        OP_Comma,           // ,
        OP_Colon,           // :
        OP_Question,        // ?
        
        // Delimiters
        DEL_LParen,         // (
        DEL_RParen,         // )
        DEL_LBrace,         // {
        DEL_RBrace,         // }
        DEL_LBracket,       // [
        DEL_RBracket,       // ]
        
        // Special
        EndOfFile,
        Unknown
    };

    struct Token {
        TokenType type;
        std::string value;
        int line;
        int column;
    };

    // Convert TokenType to a readable string for logging/debugging
    inline const char* tokenTypeToString(TokenType t) {
        switch (t) {
            // Literals
            case TokenType::Identifier: return "Identifier";
            case TokenType::Number: return "Number";
            case TokenType::String: return "String";

            // Keywords
            case TokenType::KW_Use: return "KW_Use";
            case TokenType::KW_For: return "KW_For";
            case TokenType::KW_In: return "KW_In";
            case TokenType::KW_Range: return "KW_Range";
            case TokenType::KW_If: return "KW_If";
            case TokenType::KW_Else: return "KW_Else";
            case TokenType::KW_Else_If: return "KW_Elif";
            case TokenType::KW_End: return "KW_End";
            case TokenType::KW_Update: return "KW_Update";
            case TokenType::KW_Function: return "KW_Function";
            case TokenType::KW_Return: return "KW_Return";
            case TokenType::KW_While: return "KW_While";
            case TokenType::KW_Continue: return "KW_Continue";
            case TokenType::KW_Break: return "KW_Break";
            case TokenType::KW_Match: return "KW_Match";
            case TokenType::KW_Case: return "KW_Case";
            case TokenType::KW_Default: return "KW_Default";
            case TokenType::KW_On: return "KW_On";
            case TokenType::KW_And: return "KW_And";
            case TokenType::KW_Or: return "KW_Or";
            case TokenType::KW_Not: return "KW_Not";
            case TokenType::KW_True: return "KW_True";
            case TokenType::KW_False: return "KW_False";
            case TokenType::KW_Null: return "KW_Null";
            case TokenType::KW_Print: return "KW_Print";
            case TokenType::KW_Exit: return "KW_Exit";

            // Operators
            case TokenType::OP_Assign: return "OP_Assign";
            case TokenType::OP_Plus: return "OP_Plus";
            case TokenType::OP_Minus: return "OP_Minus";
            case TokenType::OP_Star: return "OP_Star";
            case TokenType::OP_Power: return "OP_Power";
            case TokenType::OP_Slash: return "OP_Slash";
            case TokenType::OP_Percent: return "OP_Percent";
            case TokenType::OP_Plus_Assign: return "OP_Plus_Assign";
            case TokenType::OP_Minus_Assign: return "OP_Minus_Assign";
            case TokenType::OP_Percent_Assign: return "OP_Percent_Assign";
            case TokenType::OP_Increment: return "OP_Increment";
            case TokenType::OP_Decrement: return "OP_Decrement";
            case TokenType::OP_Equal: return "OP_Equal";
            case TokenType::OP_Not_Equal: return "OP_Not_Equal";
            case TokenType::OP_Less: return "OP_Less";
            case TokenType::OP_Less_Equal: return "OP_Less_Equal";
            case TokenType::OP_Greater: return "OP_Greater";
            case TokenType::OP_Greater_Equal: return "OP_Greater_Equal";
            case TokenType::OP_Bitwise_And: return "OP_Bitwise_And";
            case TokenType::OP_Bitwise_Or: return "OP_Bitwise_Or";
            case TokenType::OP_Bitwise_Xor: return "OP_Bitwise_Xor";
            case TokenType::OP_Bitwise_Not: return "OP_Bitwise_Not";
            case TokenType::OP_Left_Shift: return "OP_Left_Shift";
            case TokenType::OP_Right_Shift: return "OP_Right_Shift";
            case TokenType::OP_Dot: return "OP_Dot";
            case TokenType::OP_Comma: return "OP_Comma";
            case TokenType::OP_Colon: return "OP_Colon";
            case TokenType::OP_Question: return "OP_Question";

            // Delimiters
            case TokenType::DEL_LParen: return "DEL_LParen";
            case TokenType::DEL_RParen: return "DEL_RParen";
            case TokenType::DEL_LBrace: return "DEL_LBrace";
            case TokenType::DEL_RBrace: return "DEL_RBrace";
            case TokenType::DEL_LBracket: return "DEL_LBracket";
            case TokenType::DEL_RBracket: return "DEL_RBracket";

            // Special
            case TokenType::EndOfFile: return "EndOfFile";
            default: return "Unknown";
        }
    }

    class Lexer {
    public:
        explicit Lexer(const std::string &source) : source(source), position(0), line(1), column(1) {}

        Token getNextToken();
        std::vector<Token> tokenize();
        
    private:
        std::string source;
        size_t position;
        int line;
        int column;
        
        static const std::unordered_map<std::string, TokenType> keywords;
        
        char peek() const;
        char get();
        void skipWhitespace();
        void skipComment();
        
        Token scanString();
        Token scanNumber();
        Token scanIdentifierOrKeyword();
        Token scanOperator();
    };

} // namespace ParteeEngine