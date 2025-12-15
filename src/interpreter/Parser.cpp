#include <cstddef>

#include "interpreter/Parser.hpp"
#include "interpreter/AST.hpp"
#include "interpreter/Lexer.hpp"

namespace ParteeEngine {

	// Cursor impl
	const Token& Parser::Cursor::peek() const { return (*tokens)[pos]; }
	const Token& Parser::Cursor::advance() { return (*tokens)[pos++]; }
	bool Parser::Cursor::match(TokenType t) {
		if (atEnd()) return false;
		if (peek().type == t) { advance(); return true; }
		return false;
	}
	bool Parser::Cursor::atEnd() const { return pos >= tokens->size() || peek().type == TokenType::EndOfFile; }

	Expr* Parser::parsePrimary(Cursor& cur) {
		if (cur.atEnd()) return nullptr;

		const Token& token = cur.peek();
		switch (token.type) {
			case TokenType::Number: {
				auto* node = new NumberLiteralExpr;
                node->value = token.value;
				cur.advance();
				return node;
			}
			case TokenType::Identifier: {
				auto* node = new IdentifierExpr;
                node->name = token.value;
				cur.advance();
				return node;
			}
            case TokenType::DEL_LBracket: {
                cur.advance(); // consume '['
                auto* arrayLit = new ArrayLiteralExpr;
                // Parse elements until ']'
                if (!cur.match(TokenType::DEL_RBracket)) {
                    while (true) {
                        Expr* elem = parseExpr(cur, 0);
                        if (!elem) { delete arrayLit; return nullptr; }
                        arrayLit->elements.push_back(elem);
                        if (cur.match(TokenType::DEL_RBracket)) break;
                        if (!cur.match(TokenType::OP_Comma)) { delete arrayLit; return nullptr; }
                    }
                }
                return arrayLit;
            }
			default:
				return nullptr;
		}
	}

	Expr* Parser::parseExpr(Cursor& cursor, int minBP) {
		Expr* left = parsePrimary(cursor);
		if (!left) return nullptr;

		while (!cursor.atEnd()) {
			const Token& operatorToken = cursor.peek();

			int rightBP = 0;
			int leftBP = getInfixOperatorBindingPower(operatorToken.type, rightBP);
			if (leftBP < minBP || leftBP == 0) break;

			// Function call: foo(a, b)
			if (operatorToken.type == TokenType::DEL_LParen) {
				cursor.advance(); // consume '('
				auto* call = new CallExpr;
				call->callee = left;
				// Parse argument list
				if (!cursor.match(TokenType::DEL_RParen)) {
					while (true) {
						Expr* arg = parseExpr(cursor, 0);
						if (!arg) { delete call; return nullptr; }
						call->arguments.push_back(arg);
						if (cursor.match(TokenType::DEL_RParen)) break;
						if (!cursor.match(TokenType::OP_Comma)) { delete call; return nullptr; }
					}
				}
				left = call;
                // cursor.advance(); // consume ')'
				continue;
			}

			// Array indexing: arr[expr]
			if (operatorToken.type == TokenType::DEL_LBracket) {
				cursor.advance(); // consume '['
				Expr* index = parseExpr(cursor, 0);
				if (!index) { delete left; return nullptr; }
				if (!cursor.match(TokenType::DEL_RBracket)) { delete left; delete index; return nullptr; }
				auto* idx = new IndexExpr;
				idx->base = left;
				idx->index = index;
				left = idx;
                // cursor.advance(); // consume ']'
				continue;
			}

			// All other infix operators
			cursor.advance(); // consume operator
			Expr* rightExpression = parseExpr(cursor, rightBP);
			if (!rightExpression) { delete left; return nullptr; }

			auto* subtree = new BinaryExpr;
			subtree->operatorType = operatorToken.type;
			subtree->leftExpression = left;
			subtree->rightExpression = rightExpression;
			left = subtree;
		}
		return left;
	}

	// Parse a full program (list of top-level expressions/statements)
	ParseResult Parser::parseExpression(const std::vector<Token>& tokens) {
		Cursor cursor; cursor.tokens = &tokens; cursor.pos = 0;
		Program* program = parseProgram(cursor);
		if (!program || program->statements.empty()) {
			delete program;
			return {nullptr, false, "Failed to parse program"};
		}
		// Return the program node as an Expr* for compatibility
		return {reinterpret_cast<Expr*>(program), true, nullptr};
	}

    Program* Parser::parseProgram(Cursor &cursor) {
        auto *program = new Program;
        while (!cursor.atEnd()) {
            Expr *expr = parseExpr(cursor, 0);
            if (expr)
                program->statements.push_back(expr);
        //    if (!expr && !cursor.atEnd())
        //         cursor.advance();
        }
        return program;
    }

} // namespace ParteeEngine
