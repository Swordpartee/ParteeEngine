#include "engineinterpreter/Lexer.hpp"

#include <cctype>
#include <vector>

namespace interpreter {

    const std::unordered_map<std::string, TokenType> Lexer::keywords = {
        {"use", TokenType::KW_Use},
        {"for", TokenType::KW_For},
        {"in", TokenType::KW_In},
        {"if", TokenType::KW_If},
        {"else", TokenType::KW_Else},
        {"elif", TokenType::KW_Else_If},
        {"end", TokenType::KW_End},
        {"function", TokenType::KW_Function},
        {"while", TokenType::KW_While},
        {"continue", TokenType::KW_Continue},
        {"break", TokenType::KW_Break},
        {"return", TokenType::KW_Return},
        {"match", TokenType::KW_Switch},
        {"case", TokenType::KW_Case},
        {"default", TokenType::KW_Default},
        {"on", TokenType::KW_On},
        {"and", TokenType::KW_And},
        {"or", TokenType::KW_Or},
        {"not", TokenType::KW_Not},
        {"true", TokenType::KW_True},
        {"false", TokenType::KW_False},
        {"null", TokenType::KW_Null}
    };

    char Lexer::peek() const {
        if (position >= source.size()) {
            return '\0';
        }
        return source[position];
    }

    char Lexer::get() {
        if (position >= source.size()) {
            return '\0';
        }
        char currentChar = source[position++];
        if (currentChar == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return currentChar;
    }

    void Lexer::skipWhitespace() {
        while (isspace(peek())) {
            get();
        }
    }

    void Lexer::skipComment() {
        if (peek() == '#') {
            while (peek() != '\n' && peek() != '\0') {
                get();
            }
        }
    }

    Token Lexer::scanString() {
        int startLine = line;
        int startColumn = column;
        char quote = get(); // consume opening quote
        std::string value;

        while (peek() != quote && peek() != '\0') {
            if (peek() == '\\') {
                get();
                switch (peek()) {
                    case 'n': value += '\n'; get(); break;
                    case 't': value += '\t'; get(); break;
                    case 'r': value += '\r'; get(); break;
                    case '\\': value += '\\'; get(); break;
                    case '"': value += '"'; get(); break;
                    default: value += get(); break;
                }
            } else {
                value += get();
            }
        }

        if (peek() == quote) {
            get(); // consume closing quote
        }

        return Token{TokenType::String, value, startLine, startColumn};
    }

    Token Lexer::scanNumber() {
        int startLine = line;
        int startColumn = column;
        std::string value;

        // Handle negative numbers
        if (peek() == '-') {
            value += get(); // consume '-'
        }

        // Integer part
        while (isdigit(peek())) {
            value += get();
        }

        // Decimal part
        if (peek() == '.' && isdigit(source[position + 1])) {
            value += get(); // consume '.'
            while (isdigit(peek())) {
                value += get();
            }
        }

        return Token{TokenType::Number, value, startLine, startColumn};
    }

    Token Lexer::scanIdentifierOrKeyword() {
        int startLine = line;
        int startColumn = column;
        std::string value;

        while (isalnum(peek()) || peek() == '_') {
            value += get();
        }

        // Check if it's a keyword
        auto it = keywords.find(value);
        if (it != keywords.end()) {
            return Token{it->second, value, startLine, startColumn};
        }

        return Token{TokenType::Identifier, value, startLine, startColumn};
    }

    Token Lexer::scanOperator() {
        int startLine = line;
        int startColumn = column;
        char ch = get();

        switch (ch) {
            case '=':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Equal, "==", startLine, startColumn};
                }
                return Token{TokenType::OP_Assign, "=", startLine, startColumn};

            case '!':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Not_Equal, "!=", startLine, startColumn};
                }
                return Token{TokenType::Unknown, "!", startLine, startColumn};

            case '<':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Less_Equal, "<=", startLine, startColumn};
                }
                return Token{TokenType::OP_Less, "<", startLine, startColumn};

            case '>':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Greater_Equal, ">=", startLine, startColumn};
                }
                return Token{TokenType::OP_Greater, ">", startLine, startColumn};

            case '+':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Plus_Assign, "+=", startLine, startColumn};
                }
                if (peek() == '+') {
                    get();
                    return Token{TokenType::OP_Increment, "++", startLine, startColumn};
                }
                return Token{TokenType::OP_Plus, "+", startLine, startColumn};

            case '-':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Minus_Assign, "-=", startLine, startColumn};
                }
                if (peek() == '-') {
                    get();
                    return Token{TokenType::OP_Decrement, "--", startLine, startColumn};
                }
                return Token{TokenType::OP_Minus, "-", startLine, startColumn};

            case '*':
                if (peek() == '*') {
                    get();
                    return Token{TokenType::OP_Power, "**", startLine, startColumn};
                }
                return Token{TokenType::OP_Star, "*", startLine, startColumn};
            case '/':
                return Token{TokenType::OP_Slash, "/", startLine, startColumn};
            case '%':
                if (peek() == '=') {
                    get();
                    return Token{TokenType::OP_Percent_Assign, "%=", startLine, startColumn};
                }
                return Token{TokenType::OP_Percent, "%", startLine, startColumn};
            case '.':
                return Token{TokenType::OP_Dot, ".", startLine, startColumn};
            case ',':
                return Token{TokenType::OP_Comma, ",", startLine, startColumn};
            case ':':
                return Token{TokenType::OP_Colon, ":", startLine, startColumn};
            case '?':
                return Token{TokenType::OP_Question, "?", startLine, startColumn};
            case '&':
                return Token{TokenType::OP_Bitwise_And, "&", startLine, startColumn};
            case '|':
                return Token{TokenType::OP_Bitwise_Or, "|", startLine, startColumn};
            case '^':
                return Token{TokenType::OP_Bitwise_Xor, "^", startLine, startColumn};
            case '~':
                return Token{TokenType::OP_Bitwise_Not, "~", startLine, startColumn};
            case '(':
                return Token{TokenType::DEL_LParen, "(", startLine, startColumn};
            case ')':
                return Token{TokenType::DEL_RParen, ")", startLine, startColumn};
            case '{':
                return Token{TokenType::DEL_LBrace, "{", startLine, startColumn};
            case '}':
                return Token{TokenType::DEL_RBrace, "}", startLine, startColumn};
            case '[':
                return Token{TokenType::DEL_LBracket, "[", startLine, startColumn};
            case ']':
                return Token{TokenType::DEL_RBracket, "]", startLine, startColumn};

            default:
                return Token{TokenType::Unknown, std::string(1, ch), startLine, startColumn};
        }
    }

    Token Lexer::getNextToken() {
        skipWhitespace();

        while (peek() == '#') {
            skipComment();
            skipWhitespace();
        }

        if (peek() == '\0') {
            return Token{TokenType::EndOfFile, "", line, column};
        }

        if (peek() == '"' || peek() == '\'') {
            return scanString();
        }

        if (isdigit(peek()) || (peek() == '-' && isdigit(source[position + 1]))) {
            return scanNumber();
        }

        if (isalpha(peek()) || peek() == '_') {
            return scanIdentifierOrKeyword();
        }

        return scanOperator();
    }

    std::vector<Token> Lexer::tokenize() {
        std::vector<Token> tokens;
        Token token = getNextToken();

        while (token.type != TokenType::EndOfFile) {
            tokens.push_back(token);
            token = getNextToken();
        }

        tokens.push_back(token); // Add EOF token
        return tokens;
    }

} // namespace interpreter