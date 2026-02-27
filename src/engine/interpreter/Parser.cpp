#include "engine/interpreter/Parser.hpp"

#include "engine/interpreter/AST.hpp"
#include "engine/interpreter/Lexer.hpp"

#include <cstddef>
#include <memory>

#define ERROR_LOCATION(cursor) " at line " << cursor.peek().line << ", column " << cursor.peek().column

namespace interpreter {
	// Cursor impl
	const Token& Parser::Cursor::peek() const { return (*tokens)[pos]; }
	const Token& Parser::Cursor::advance() { return (*tokens)[pos++]; }
	bool Parser::Cursor::match(TokenType t) {
		if (atEnd()) return false;
		if (peek().type == t) { advance(); return true; }
		return false;
	}
	bool Parser::Cursor::atEnd() const { return pos >= tokens->size() || peek().type == TokenType::EndOfFile; }

	void Parser::enterContext(ParsingContext context) {
    	contextStack.push(context);
	}

	void Parser::exitContext() {
		if (!contextStack.empty()) {
			contextStack.pop();
		}
	}

	ParsingContext Parser::currentContext() {
		return contextStack.empty() ? ParsingContext::Global : contextStack.top();
	}

	bool Parser::inContext(ParsingContext context) {
		std::stack<ParsingContext> tempStack = contextStack;
		while (!tempStack.empty()) {
			if (tempStack.top() == context) {
				return true;
			}
			tempStack.pop();
		}
		return false;
	}

	Expr* Parser::parseExpr(Cursor& cursor, int minBP) {
		Expr* left = nullptr;
		const Token& token = cursor.advance();

		switch (token.type) {
			// Special constructs
			case TokenType::OP_Bitwise_Not: {
				return new UnaryExpr{token.type, parseExpr(cursor, 0)};
			}
			case TokenType::KW_Not: {
				return new UnaryExpr{token.type, parseExpr(cursor, 0)};
			}
			case TokenType::KW_Use: {
				if (cursor.peek().type != TokenType::String) { return reportError("Expected module name (string) after 'use' keyword", cursor); }
				return new UseExpr{ new StringLiteral{cursor.advance().value} };
			}
			case TokenType::KW_On: {
				return parseOnExpr(cursor);
			}
			case TokenType::KW_If: {
				return parseIfExpr(cursor);
			}
			case TokenType::KW_For: {
				return parseForLoop(cursor);
			}
			case TokenType::KW_While: {
				return parseWhileLoop(cursor);
			}
			case TokenType::KW_Function: {
				return parseFunctionExpr(cursor);
			}
			case TokenType::KW_Switch: {
				return reportError("Switch statement parsing not yet implemented", cursor);
			}
			// Keywords 
			case TokenType::KW_Return: {
				if (!inContext(ParsingContext::Function)) { return reportError("Return statement outside of function context", cursor); }
				return new ReturnExpr{parseExpr(cursor, 0)};
			}	
			case TokenType::KW_Break: {
				if (!inContext(ParsingContext::Loop)) { return reportError("Break statement outside of loop context", cursor); }
				return new BreakExpr();
			}
			case TokenType::KW_Continue: {
				if (!inContext(ParsingContext::Loop)) { return reportError("Continue statement outside of loop context", cursor); }
				return new ContinueExpr();
			}
			// Primary expressions
			case TokenType::Number: {
				left = new NumberLiteral{token.value};
				break;
			}
			case TokenType::String: {
				left = new StringLiteral{token.value};
				break;
			}
			case TokenType::DEL_LBracket: {
				left = parseArrayLiteral(cursor);
				break;
			}
			case TokenType::KW_True: {
				left = new BooleanLiteral(true);
				break;
			}
			case TokenType::KW_False: {
				left = new BooleanLiteral(false);
				break;
			}
			case TokenType::Identifier: {
				left = new Identifier{token.value};
				break;
			}
			case TokenType::DEL_LParen: {
				left = parseExpr(cursor, 0);
				if (!cursor.match(TokenType::DEL_RParen)) {
					return reportError("Expected ')' after expression", cursor);
				}
				break;
			}
			default: {
				return reportError("Unexpected token in primary expression: " + token.value, cursor);
			}
		}

			// Handle postfix operations (e.g., member access, index access)
		while (!cursor.atEnd()) {

			left = parseIndexExpr(cursor, left);

			left = parseCallExpr(cursor, left);

			int rightBP;
			int leftBP = getInfixOperatorBindingPower(cursor.peek().type, rightBP);

			// If the operator's precedence is lower than the current minimum, stop parsing
			if (leftBP == 0 || leftBP < minBP) { break; }

			left = parseBinaryExpr(cursor, left, minBP);
		}
		
		return left;
	}

	std::vector<Expr*> Parser::parseArguments(Cursor& cursor) {
		std::vector<Expr*> args;

		if (cursor.match(TokenType::DEL_RParen)) {
			return args; // empty argument list
		}

		while (!cursor.atEnd()) {
			Expr* arg = parseExpr(cursor, 0);
			if (!arg) {
				std::cerr << RED << "Failed to parse argument in function call" << GREY << ERROR_LOCATION(cursor) << RESET << "\n";
				synchronize(cursor);
				for (auto* a : args) delete a;
				return {};
			}
			args.push_back(arg);
			if (cursor.match(TokenType::DEL_RParen))
				break;
			if (!cursor.match(TokenType::OP_Comma)) {
				std::cerr << RED << "Expected ',' or ')' in argument list" << GREY << ERROR_LOCATION(cursor) << RESET << "\n";
				synchronize(cursor);
				for (auto* a : args) delete a;
				return {};
			}
		}
		return args;
	}

	Expr* Parser::parseBodyExpr(Cursor& cursor) {
		auto* body = new BodyExpr();

		while (!cursor.atEnd() && !cursor.match(TokenType::KW_End) && !(cursor.peek().type == TokenType::KW_Else)) {
			Expr* stmt = parseExpr(cursor, 0);
			if (stmt)
				body->statements.push_back(stmt);
			else {
				return reportError("Failed to parse statement in body", cursor);
			}
		}

		return body;
	}

	Expr* Parser::parseArrayLiteral(Cursor& cursor) {
		auto* arrayLit = new ArrayLiteral;

		if (cursor.match(TokenType::DEL_RBracket)) {return arrayLit;}

		while (!cursor.atEnd()) {
			Expr* elem = parseExpr(cursor, 0);
			if (!elem) { 
				delete arrayLit; 
				return reportError("Failed to parse array element", cursor);
			}
			arrayLit->elements.push_back(elem);
			if (cursor.match(TokenType::DEL_RBracket)) break;
			if (!cursor.match(TokenType::OP_Comma)) { 
				delete arrayLit; 
				return reportError("Expected ',' or ']' in array literal", cursor);
			}
		}

		return arrayLit;
	}

	Expr* Parser::parseEventLiteral(Cursor& cursor) {
		auto* eventLit = new EventLiteral;

		eventLit->eventName = parseExpr(cursor, 0);
		if (!eventLit->eventName) {
			delete eventLit;
			return reportError("Failed to parse event name in event literal", cursor);
		}

		if (!cursor.match(TokenType::DEL_LBrace)) {
			delete eventLit;
			return reportError("Expected '{' after event name in event literal", cursor);
		}

		if (cursor.match(TokenType::DEL_RBrace)) {
			return eventLit; // no parameters
		}

		while (!cursor.atEnd()) {
			Expr* param = parseExpr(cursor, 0);
			if (!param) {
				delete eventLit;
				return reportError("Failed to parse parameter in event literal", cursor);
			}
			eventLit->parameters.push_back(param);
			if (cursor.match(TokenType::DEL_RBrace)) break;
			if (!cursor.match(TokenType::OP_Comma)) {
				delete eventLit;
				return reportError("Expected ',' or '}' in event literal parameter list", cursor);
			}
		}
		return eventLit;
	}

	Expr* Parser::parseBinaryExpr(Cursor& cursor, Expr* left, int minBP) {
		while (!cursor.atEnd()) {
			const Token& operatorToken = cursor.peek();

			// Get the binding power (precedence) of the operator
			int rightBP;
			int leftBP = getInfixOperatorBindingPower(operatorToken.type, rightBP);

			// If the operator's precedence is lower than the current minimum, stop parsing
			if (leftBP == 0 || leftBP < minBP) { return left; }

			// Consume the operator token
			cursor.advance();

			// Parse the right-hand side of the expression
			Expr* right = parseExpr(cursor, rightBP);

			// Create a new BinaryExpr node
			left = new BinaryExpr(operatorToken.type, left, right);	

		}

		return left;
	}

	Expr* Parser::parseCallExpr(Cursor& cursor, Expr* left) {		
		if (!cursor.match(TokenType::DEL_LParen)) { return left; }

		std::vector<Expr*> args = parseArguments(cursor);

		return new CallExpr{left, args};
	}

	Expr* Parser::parseIndexExpr(Cursor& cursor, Expr* left) {
		if (!cursor.match(TokenType::DEL_LBracket)) { return left; }
		Expr* index = parseExpr(cursor, 0);
		if (!index) {
			delete left;
			return reportError("Failed to parse index expression", cursor);
		}
		if (!cursor.match(TokenType::DEL_RBracket)) {
			delete left;
			delete index;
			return reportError("Expected ']' after index expression", cursor);
		}
		auto* idx = new IndexExpr();
		idx->base = left;
		idx->index = index;
		return idx;
	}

	Expr* Parser::parseOnExpr(Cursor& cursor) {
		auto* onExpr = new OnExpr();

		onExpr->eventLiteral = parseEventLiteral(cursor);
		if (!onExpr->eventLiteral) {
			delete onExpr;
			return reportError("Failed to parse event literal in 'on' expression", cursor);
		}

		onExpr->body = parseBodyExpr(cursor);
		if (!onExpr->body) {
			delete onExpr;
			return reportError("Failed to parse body in 'on' expression", cursor);
		}

		return onExpr;
	}

	Expr* Parser::parseIfExpr(Cursor& cursor) {
		auto* ifExpr = new IfExpr();
		enterContext(ParsingContext::Conditional);

		ifExpr->condition = parseExpr(cursor, 0);
		if (!ifExpr->condition) {
			delete ifExpr;
			return reportError("Failed to parse condition in 'if' expression", cursor);
		}

		ifExpr->thenBranch = parseBodyExpr(cursor);

		// Check for 'else' or 'else if'
    	if (cursor.match(TokenType::KW_Else)) {
        // Check if it's an 'else if'
			if (cursor.match(TokenType::KW_If)) {
				ifExpr->elseBranch = parseIfExpr(cursor); // Recursively parse 'else if'
			} else {
				ifExpr->elseBranch = parseBodyExpr(cursor); // Parse the 'else' branch
			}

		}

		return ifExpr;
	}

	Expr* Parser::parseForLoop(Cursor& cursor) {
		auto* forExpr = new ForExpr();
		enterContext(ParsingContext::Loop);

		if (cursor.peek().type != TokenType::Identifier) {
			delete forExpr;
			return reportError("Expected iterator name (identifier) after 'for' keyword", cursor);
		}
		forExpr->iteratorName = new Identifier{cursor.advance().value};

		if (!cursor.match(TokenType::KW_In)) {
			delete forExpr;
			return reportError("Expected 'in' after iterator name in for loop", cursor);
		}

		forExpr->iterable = parseExpr(cursor, 0);
		if (!forExpr->iterable) {
			delete forExpr;
			return reportError("Failed to parse iterable expression in for loop", cursor);
		}

		forExpr->body = parseBodyExpr(cursor);
		if (!forExpr->body) {
			delete forExpr;
			return reportError("Failed to parse body in for loop", cursor);
		}

		return forExpr;
	}

	Expr* Parser::parseWhileLoop(Cursor& cursor) {
		auto* whileExpr = new WhileExpr();
		enterContext(ParsingContext::Loop);

		whileExpr->condition = parseExpr(cursor, 0);

		if (!whileExpr->condition) {
			delete whileExpr;
			return reportError("Failed to parse condition in while loop", cursor);
		}

		whileExpr->body = parseBodyExpr(cursor);
		if (!whileExpr->body) {
			delete whileExpr;
			return reportError("Failed to parse body in while loop", cursor);
		}

		return whileExpr;
	}
	
	Expr* Parser::parseFunctionExpr(Cursor& cursor) {
		auto* funcExpr = new FunctionExpr();
		enterContext(ParsingContext::Function);

		if (cursor.peek().type != TokenType::Identifier) {
			delete funcExpr;
			return reportError("Expected function name (identifier) after 'function' keyword", cursor);
		}
		funcExpr->name = new Identifier{cursor.advance().value};

		if (!cursor.match(TokenType::DEL_LParen)) {
			delete funcExpr;
			return reportError("Expected '(' after function name", cursor);
		}

		// Parse parameters
		funcExpr->parameters = parseArguments(cursor);

		funcExpr->body = parseBodyExpr(cursor);
		if (!funcExpr->body) {
			delete funcExpr;
			return reportError("Failed to parse function body", cursor);
		}

		return funcExpr;
	}

	Program *Parser::parseProgram(Cursor &cursor) {
		Program *program = new Program();

		while (!cursor.atEnd()) {
			Expr *expr = parseExpr(cursor, 0);
			if (!expr) {
				std::cerr << RED << "Failed to parse expression in program." << GREY << ERROR_LOCATION(cursor) << RESET << "\n";
			}

			program->statements.push_back(expr);
		}

		return program;
	}

	ParseResult Parser::parse(const std::vector<Token>& tokens) {
		Cursor cursor; cursor.tokens = &tokens; cursor.pos = 0;
		errorReported = false;

		Program* program = parseProgram(cursor);

		if (!program || program->statements.empty()) {
			delete program;
			return {nullptr, false, "Failed to parse program"};
		}

		// Return the program node as an Expr* for compatibility
		return {program, !errorReported, nullptr};
	}

	void Parser::synchronize(Cursor& cursor) {
        while (!cursor.atEnd()) {
            // Advance the cursor to skip the problematic token
            cursor.advance();

           	if (!cursor.atEnd() && cursor.match(TokenType::KW_End)) {
				return;
            }
        }
    }

	Expr* Parser::reportError(const std::string& message, Cursor& cursor) {
		errorReported = true;
		Token currentToken = cursor.peek();
		std::cerr << RED << "Parse Error: " << message << GREY << ERROR_LOCATION(cursor) << RESET << "\n";
		synchronize(cursor);
		return new ErrorExpr(message, currentToken.line, currentToken.column);
	}

}