#include "engineinterpreter/Interpreter.hpp"

namespace interpreter {

    void Environment::define(const std::string& name, const Value& value) {
        if (variables.find(name) != variables.end()) {
            throw std::runtime_error("Variable already defined: " + name);
        }
        variables[name] = value;
    }

    void Environment::set(const std::string& name, const Value& value) {
        if (variables.find(name) != variables.end()) {
            variables[name] = value;
        } else if (parent) {
            parent->set(name, value);
        } else {
            define(name, value);
            // throw std::runtime_error("Undefined variable: " + name);
        }
    }

    Value Environment::get(const std::string& name) {
        if (variables.find(name) != variables.end()) {
            return variables[name];
        } else if (parent) {
            return parent->get(name);
        } else {
            throw std::runtime_error("Undefined variable: " + name);
        }
    }

    bool Environment::has(const std::string& name) const {
        if (variables.find(name) != variables.end()) {
            return true;
        } else if (parent) {
            return parent->has(name);
        } else {
            return false;
        }
    }

    ReturnSignal::ReturnSignal(const Value& val) : returnValue(val) {}

    void Interpreter::interpret(const std::string& source, Engine* enginePtr) {
        this->engine = enginePtr;
        Lexer lexer(ScriptLoader::loadScript(source));
        auto tokens = lexer.tokenize();

        Parser parser;
        auto parseResult = parser.parse(tokens);

        if (!parseResult.ok) {
            std::cerr << "Parse error: " << parseResult.message << "\n";
            return;
        } 

        // printAST(parseResult.program); // Debug: print the AST

        registerGlobals();
        execute(parseResult.program);
    }

    void Interpreter::registerGlobals() {
        auto makeNative = [](NativeFunction fn) {
            auto func = std::make_shared<ScriptFunction>();
            func->native = fn;
            return Value(func);
        };

        globalEnv->define("print", makeNative([](std::vector<Value> args) {
            for (auto& arg : args) {
                switch (arg.type) {
                    case Value::Type::Number:
                        std::cout << std::get<double>(arg.data);
                        break;
                    case Value::Type::String:
                        std::cout << std::get<std::string>(arg.data);
                        break;
                    case Value::Type::Boolean:
                        std::cout << (std::get<bool>(arg.data) ? "true" : "false");
                        break;
                    case Value::Type::Null:
                        std::cout << "null";
                        break;
                    default:
                        std::cout << "<value>";
                }
            }
            std::cout << std::endl;
            return Value{};
        }));

        globalEnv->define("len", makeNative([](std::vector<Value> args) {
            if (args[0].type == Value::Type::Array)
                return Value((double)std::get<Value::Array>(args[0].data)->size());
            if (args[0].type == Value::Type::String)
                return Value((double)std::get<std::string>(args[0].data).size());
            throw std::runtime_error("len() expects array or string");
        }));
    }

    void Interpreter::execute(Program* program) {
        for (auto* stmt : program->statements) {
            execute(stmt, globalEnv.get());
        }
    }

    Value Interpreter::execute(Expr* expr, Environment* env) {
        if (!expr) return Value();
        
        switch(expr->getType()) {
            case ExprType::Error:
                return execute(static_cast<ErrorExpr*>(expr), env);
            case ExprType::NumberLiteral:
                return execute(static_cast<NumberLiteral*>(expr), env);
            case ExprType::StringLiteral:
                return execute(static_cast<StringLiteral*>(expr), env);
            case ExprType::BooleanLiteral:
                return execute(static_cast<BooleanLiteral*>(expr), env);
            case ExprType::Identifier:
                return execute(static_cast<Identifier*>(expr), env);
            case ExprType::ArrayLiteral:
                return execute(static_cast<ArrayLiteral*>(expr), env);
            case ExprType::EventLiteral:
                return execute(static_cast<EventLiteral*>(expr), env);
            case ExprType::UnaryExpr:
                return execute(static_cast<UnaryExpr*>(expr), env);
            case ExprType::BinaryExpr:
                return execute(static_cast<BinaryExpr*>(expr), env);
            case ExprType::BodyExpr:
                return execute(static_cast<BodyExpr*>(expr), env);
            case ExprType::UseExpr:
                return execute(static_cast<UseExpr*>(expr), env);
            case ExprType::OnExpr:
                return execute(static_cast<OnExpr*>(expr), env);
            case ExprType::ReturnExpr:
                return execute(static_cast<ReturnExpr*>(expr), env);
            case ExprType::ContinueExpr:
                return execute(static_cast<ContinueExpr*>(expr), env);
            case ExprType::BreakExpr:
                return execute(static_cast<BreakExpr*>(expr), env);
            case ExprType::CaseExpr:
                return execute(static_cast<CaseExpr*>(expr), env);
            case ExprType::DefaultExpr:
                return execute(static_cast<DefaultExpr*>(expr), env);
            case ExprType::SwitchExpr:
                return execute(static_cast<SwitchExpr*>(expr), env);
            case ExprType::IfExpr:
                return execute(static_cast<IfExpr*>(expr), env);
            case ExprType::WhileExpr:
                return execute(static_cast<WhileExpr*>(expr), env);
            case ExprType::ForExpr:
                return execute(static_cast<ForExpr*>(expr), env);
            case ExprType::FunctionExpr:
                return execute(static_cast<FunctionExpr*>(expr), env);
            case ExprType::CallExpr:
                return execute(static_cast<CallExpr*>(expr), env);
            case ExprType::IndexExpr:
                return execute(static_cast<IndexExpr*>(expr), env);
            default:
                throw std::runtime_error("Unknown expression type");
        }
    }

    Value Interpreter::execute([[maybe_unused]]ErrorExpr* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Error expression: " + expr->message);
    }

    Value Interpreter::execute(NumberLiteral* expr, [[maybe_unused]]Environment* env) {
        return Value(expr->value);
    }

    Value Interpreter::execute(StringLiteral* expr, [[maybe_unused]]Environment* env) {
        return Value(expr->value);
    }

    Value Interpreter::execute(BooleanLiteral* expr, [[maybe_unused]]Environment* env) {
        return Value(expr->value);
    }

    Value Interpreter::execute(Identifier* expr, [[maybe_unused]]Environment* env) {
        return env->get(expr->name);
    }

    Value Interpreter::execute(ArrayLiteral* expr, [[maybe_unused]]Environment* env) {
        Value::Array arr = std::make_shared<std::vector<Value>>();
        for (auto* elem : expr->elements) {
            arr->push_back(execute(elem, env));
        }
        return Value(arr);
    }

    Value Interpreter::execute([[maybe_unused]]EventLiteral* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Event literals are not yet implemented in the interpreter");
    }

    Value Interpreter::execute(BodyExpr* expr, [[maybe_unused]]Environment* env) {
        Value result;
        for (auto* stmt : expr->statements) {
            result = execute(stmt, env);
        }
        return result;
    }

    Value Interpreter::execute([[maybe_unused]]UseExpr* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Module imports are not yet implemented in the interpreter");
    }

    Value Interpreter::execute([[maybe_unused]]OnExpr* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Event listeners are not yet implemented in the interpreter");
    }

    Value Interpreter::execute(ReturnExpr* expr, [[maybe_unused]]Environment* env) {
        throw ReturnSignal(execute(expr->value, env));
    }

    Value Interpreter::execute([[maybe_unused]]ContinueExpr* expr, [[maybe_unused]]Environment* env) {
        throw ContinueSignal();
    }

    Value Interpreter::execute([[maybe_unused]]BreakExpr* expr, [[maybe_unused]]Environment* env) {
        throw BreakSignal();
    }

    Value Interpreter::execute([[maybe_unused]]CaseExpr* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Case expressions should only be evaluated within a switch statement");
    }

    Value Interpreter::execute([[maybe_unused]]DefaultExpr* expr, [[maybe_unused]]Environment* env) {
        throw std::runtime_error("Default expressions should only be evaluated within a switch statement");
    }

    Value Interpreter::execute([[maybe_unused]]SwitchExpr* expr, [[maybe_unused]]Environment* env) {
        Value switchVal = execute(expr->expression, env);
        for (const auto* caseExpr : expr->cases) {
            Value caseVal = execute(caseExpr->caseValue, env);
            
            if (areEqual(switchVal, caseVal)) {
                return execute(caseExpr->body, env);
            }
        }
        if (expr->defaultCase) {
            return execute(expr->defaultCase->body, env);
        } else {
            return Value(); // null value if no case matches and no default
        }
    }

    Value Interpreter::execute([[maybe_unused]]IfExpr* expr, [[maybe_unused]]Environment* env) {
        Value conditionVal = execute(expr->condition, env);
        if (conditionVal.type != Value::Type::Boolean) {
            throw std::runtime_error("Condition expression must evaluate to a boolean");
        }
        if (std::get<bool>(conditionVal.data)) {
            return execute(expr->thenBranch, env);
        } else if (expr->elseBranch) {
            return execute(expr->elseBranch, env);
        } else {
            return Value(); // null value if no else branch
        }
    }

    Value Interpreter::execute([[maybe_unused]]WhileExpr* expr, [[maybe_unused]]Environment* env) {
        Value conditionVal = execute(expr->condition, env);
        if (conditionVal.type != Value::Type::Boolean) {
            throw std::runtime_error("Condition expression must evaluate to a boolean");
        }
        while (std::get<bool>(conditionVal.data)) {
            try {
                execute(expr->body, env);
            } catch (const BreakSignal&) {
                break;
            } catch (const ContinueSignal&) {
                continue;
            }
            conditionVal = execute(expr->condition, env);
            if (conditionVal.type != Value::Type::Boolean) {
                throw std::runtime_error("Condition expression must evaluate to a boolean");
            }
        }
        return Value(); // null value after loop finishes
    }

    Value Interpreter::execute([[maybe_unused]]ForExpr* expr, [[maybe_unused]]Environment* env) {
        Value iterableVal = execute(expr->iterable, env);
        if (iterableVal.type != Value::Type::Array) {
            throw std::runtime_error("Iterable expression must evaluate to an array");
        }
        auto& arr = std::get<Value::Array>(iterableVal.data);
        for (const auto& item : *arr) {
            auto loopEnv = Environment::make(env->shared_from_this());
            loopEnv->define(static_cast<Identifier*>(expr->iteratorName)->name, item);
            try {
                execute(expr->body, loopEnv.get());
            } catch (const BreakSignal&) {
                break;
            } catch (const ContinueSignal&) {
                continue;
            }
        }
        return Value(); // null value after loop finishes
    }

    Value Interpreter::execute([[maybe_unused]]FunctionExpr* expr, [[maybe_unused]]Environment* env) {
        auto func = std::make_shared<ScriptFunction>();

        for (auto* param : expr->parameters) {
            func->params.push_back(static_cast<Identifier*>(param)->name);
        }

        func->body    = expr->body;
        func->closure = env->shared_from_this();

        Value funcVal;
        funcVal.type = Value::Type::Function;
        funcVal.data = func;

        env->define(static_cast<Identifier*>(expr->name)->name, funcVal);
        return Value{}; // definition is a statement, returns null
    }

    Value Interpreter::execute([[maybe_unused]]CallExpr* expr, [[maybe_unused]]Environment* env) {
        Value calleeVal = execute(expr->callee, env);
        if (calleeVal.type != Value::Type::Function) {
            throw std::runtime_error("Attempted to call a non-function value");
        }
        auto func = std::get<Value::Function>(calleeVal.data);
        
        std::vector<Value> argValues;
        for (auto* argExpr : expr->arguments) {
            argValues.push_back(execute(argExpr, env));
        }
        
        if (func->native) {
            return func->native(argValues);
        }

        if (expr->arguments.size() != func->params.size()) {
            throw std::runtime_error("Argument count mismatch in function call");
        }

        // Create new environment for function execution
        auto funcEnv = Environment::make(func->closure);
        for (size_t i = 0; i < func->params.size(); ++i) {
            funcEnv->define(func->params[i], argValues[i]);
        }

        try {
            return execute(func->body, funcEnv.get());
        } catch (const ReturnSignal& ret) {
            return ret.returnValue;
        }
    }

    Value Interpreter::execute([[maybe_unused]]IndexExpr* expr, [[maybe_unused]]Environment* env) {
        Value baseVal = execute(expr->base, env);
        Value indexVal = execute(expr->index, env);
        if (baseVal.type == Value::Type::Array && indexVal.type == Value::Type::Number) {
            const auto& arr = std::get<Value::Array>(baseVal.data);
            int idx = static_cast<int>(std::get<double>(indexVal.data));
            if (idx < 0 || idx >= static_cast<int>(arr->size())) {
                throw std::runtime_error("Array index out of bounds");
            }
            return (*arr)[idx];
        } else if (baseVal.type == Value::Type::String && indexVal.type == Value::Type::Number) {
            const auto& str = std::get<std::string>(baseVal.data);
            int idx = static_cast<int>(std::get<double>(indexVal.data));
            if (idx < 0 || idx >= static_cast<int>(str.size())) {
                throw std::runtime_error("String index out of bounds");
            }
            return Value(std::string(1, str[idx]));
        } else {
            throw std::runtime_error("Invalid types for indexing operation");
        }
    }

    Value Interpreter::execute(BinaryExpr* expr, Environment* env) {
        // Dot must be handled before evaluating either side
        if (expr->operatorType == TokenType::OP_Dot) {
            Value obj = execute(expr->leftExpression, env);
            if (obj.type != Value::Type::Object) {
                throw std::runtime_error("Dot operator on non-object");
            }
            return execute(expr->rightExpression, std::get<Value::Object>(obj.data).get());
        }
        // Handle assignment and increment/decrement operators specially - don't evaluate left side upfront
        if (expr->operatorType == TokenType::OP_Assign ||
            expr->operatorType == TokenType::OP_Plus_Assign ||
            expr->operatorType == TokenType::OP_Minus_Assign ||
            expr->operatorType == TokenType::OP_Percent_Assign ||
            expr->operatorType == TokenType::OP_Increment ||
            expr->operatorType == TokenType::OP_Decrement) {
            
            auto* id = dynamic_cast<Identifier*>(expr->leftExpression);
            if (!id) {
                throw std::runtime_error("Left-hand side of assignment/modification must be an identifier");
            }
            
            Value rightVal = execute(expr->rightExpression, env);
            
            switch (expr->operatorType) {
                case TokenType::OP_Assign:
                    env->set(id->name, rightVal);
                    return rightVal;
                case TokenType::OP_Plus_Assign: {
                    Value leftVal = env->get(id->name);
                    if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                        Value result(std::get<double>(leftVal.data) + std::get<double>(rightVal.data));
                        env->set(id->name, result);
                        return result;
                    }
                    throw std::runtime_error("Invalid types for '+=' operator");
                }
                case TokenType::OP_Minus_Assign: {
                    Value leftVal = env->get(id->name);
                    if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                        Value result(std::get<double>(leftVal.data) - std::get<double>(rightVal.data));
                        env->set(id->name, result);
                        return result;
                    }
                    throw std::runtime_error("Invalid types for '-=' operator");
                }
                case TokenType::OP_Percent_Assign: {
                    Value leftVal = env->get(id->name);
                    if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                        if (std::get<double>(rightVal.data) == 0) {
                            throw std::runtime_error("Division by zero in '%=' operation");
                        }
                        Value result(std::fmod(std::get<double>(leftVal.data), std::get<double>(rightVal.data)));
                        env->set(id->name, result);
                        return result;
                    }
                    throw std::runtime_error("Invalid types for '%=' operator");
                }
                case TokenType::OP_Increment: {
                    Value leftVal = env->get(id->name);
                    if (leftVal.type == Value::Type::Number) {
                        Value result(std::get<double>(leftVal.data) + 1);
                        env->set(id->name, result);
                        return result;
                    }
                    throw std::runtime_error("Invalid type for '++' operator");
                }
                case TokenType::OP_Decrement: {
                    Value leftVal = env->get(id->name);
                    if (leftVal.type == Value::Type::Number) {
                        Value result(std::get<double>(leftVal.data) - 1);
                        env->set(id->name, result);
                        return result;
                    }
                    throw std::runtime_error("Invalid type for '--' operator");
                }
                default:
                    throw std::runtime_error("Unexpected assignment/modification operator");
            }
        }
        
        
        // For all other operators, evaluate both sides
        Value leftVal = execute(expr->leftExpression, env);
        Value rightVal = execute(expr->rightExpression, env);

        switch (expr->operatorType) {
            case TokenType::OP_Plus: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) + std::get<double>(rightVal.data));
                }
                if (leftVal.type == Value::Type::String && rightVal.type == Value::Type::String) {
                    return Value(std::get<std::string>(leftVal.data) + std::get<std::string>(rightVal.data));
                }
                if (leftVal.type == Value::Type::String && rightVal.type == Value::Type::Number) {
                    return Value(std::get<std::string>(leftVal.data) + std::to_string(std::get<double>(rightVal.data)));
                }
                throw std::runtime_error("Invalid types for '+' operator");
            }
            case TokenType::OP_Minus: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) - std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '-' operator");
            }
            case TokenType::OP_Star: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) * std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '*' operator");
            }
            case TokenType::OP_Slash: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    if (std::get<double>(rightVal.data) == 0) {
                        throw std::runtime_error("Division by zero");
                    }
                    return Value(std::get<double>(leftVal.data) / std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '/' operator");
            }
            case TokenType::OP_Percent: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    if (std::get<double>(rightVal.data) == 0) {
                        throw std::runtime_error("Division by zero in modulus operation");
                    }
                    return Value(std::fmod(std::get<double>(leftVal.data), std::get<double>(rightVal.data)));
                }
                throw std::runtime_error("Invalid types for '%' operator");
            }
            case TokenType::OP_Power: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::pow(std::get<double>(leftVal.data), std::get<double>(rightVal.data)));
                }
                throw std::runtime_error("Invalid types for '**' operator");
            }
            case TokenType::OP_Equal: {
                if (leftVal.type == rightVal.type) {
                    if (leftVal.type == Value::Type::Number) {
                        return Value(std::get<double>(leftVal.data) == std::get<double>(rightVal.data));
                    } else if (leftVal.type == Value::Type::String) {
                        return Value(std::get<std::string>(leftVal.data) == std::get<std::string>(rightVal.data));
                    } else if (leftVal.type == Value::Type::Boolean) {
                        return Value(std::get<bool>(leftVal.data) == std::get<bool>(rightVal.data));
                    }
                }
                return Value(false);
            }
            case TokenType::OP_Not_Equal: {
                if (leftVal.type == rightVal.type) {
                    if (leftVal.type == Value::Type::Number) {
                        return Value(std::get<double>(leftVal.data) != std::get<double>(rightVal.data));
                    } else if (leftVal.type == Value::Type::String) {
                        return Value(std::get<std::string>(leftVal.data) != std::get<std::string>(rightVal.data));
                    } else if (leftVal.type == Value::Type::Boolean) {
                        return Value(std::get<bool>(leftVal.data) != std::get<bool>(rightVal.data));
                    }
                }
                return Value(true);
            }
            case TokenType::OP_Less: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) < std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '<' operator");
            }
            case TokenType::OP_Less_Equal: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) <= std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '<=' operator");
            }
            case TokenType::OP_Greater: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) > std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '>' operator");
            }
            case TokenType::OP_Greater_Equal: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(std::get<double>(leftVal.data) >= std::get<double>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for '>=' operator");
            }
            case TokenType::KW_And: {
                if (leftVal.type == Value::Type::Boolean && rightVal.type == Value::Type::Boolean) {
                    return Value(std::get<bool>(leftVal.data) && std::get<bool>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for 'and' operator");
            }
            case TokenType::KW_Or: {
                if (leftVal.type == Value::Type::Boolean && rightVal.type == Value::Type::Boolean) {
                    return Value(std::get<bool>(leftVal.data) || std::get<bool>(rightVal.data));
                }
                throw std::runtime_error("Invalid types for 'or' operator");
            }
            case TokenType::OP_Bitwise_And: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(static_cast<long long>(std::get<double>(leftVal.data)) & static_cast<long long>(std::get<double>(rightVal.data))));
                }
                throw std::runtime_error("Invalid types for '&' operator");
            }
            case TokenType::OP_Bitwise_Or: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(static_cast<long long>(std::get<double>(leftVal.data)) | static_cast<long long>(std::get<double>(rightVal.data))));
                }
                throw std::runtime_error("Invalid types for '|' operator");
            }
            case TokenType::OP_Bitwise_Xor: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(static_cast<long long>(std::get<double>(leftVal.data)) ^ static_cast<long long>(std::get<double>(rightVal.data))));
                }
                throw std::runtime_error("Invalid types for '^' operator");
            }
            case TokenType::OP_Left_Shift: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(static_cast<long long>(std::get<double>(leftVal.data)) << static_cast<long long>(std::get<double>(rightVal.data))));
                }
                throw std::runtime_error("Invalid types for '<<' operator");
            }
            case TokenType::OP_Right_Shift: {
                if (leftVal.type == Value::Type::Number && rightVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(static_cast<long long>(std::get<double>(leftVal.data)) >> static_cast<long long>(std::get<double>(rightVal.data))));
                }
                throw std::runtime_error("Invalid types for '>>' operator");
            }
            case TokenType::OP_Bitwise_Not: {
                if (leftVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(~static_cast<long long>(std::get<double>(leftVal.data))));
                }
                throw std::runtime_error("Invalid type for '~' operator");
            }
            // Implement other binary operators similarly
            default:
                throw std::runtime_error("Operator not implemented in binary expression execution");
        }
    }

    Value Interpreter::execute(UnaryExpr* expr, Environment* env) {
        Value operandVal = execute(expr->operand, env);

        switch (expr->operatorType) {
            case TokenType::OP_Plus:
                if (operandVal.type == Value::Type::Number) {
                    return operandVal; // unary plus does nothing
                }
                throw std::runtime_error("Invalid type for unary '+' operator");
            case TokenType::OP_Minus:
                if (operandVal.type == Value::Type::Number) {
                    return Value(-std::get<double>(operandVal.data));
                }
                throw std::runtime_error("Invalid type for unary '-' operator");
            case TokenType::KW_Not:
                if (operandVal.type == Value::Type::Boolean) {
                    return Value(!std::get<bool>(operandVal.data));
                }
                throw std::runtime_error("Invalid type for 'not' operator");
            case TokenType::OP_Bitwise_Not:
                if (operandVal.type == Value::Type::Number) {
                    return Value(static_cast<double>(~static_cast<long long>(std::get<double>(operandVal.data))));
                }
                throw std::runtime_error("Invalid type for '~' operator");
            default:
                throw std::runtime_error("Operator not implemented in unary expression execution");
        }
    }

    bool Interpreter::areEqual(const Value& a, const Value& b) {
        if (a.type != b.type) return false;
        switch (a.type) {
            case Value::Type::Number:
                return std::get<double>(a.data) == std::get<double>(b.data);
            case Value::Type::String:
                return std::get<std::string>(a.data) == std::get<std::string>(b.data);
            case Value::Type::Boolean:
                return std::get<bool>(a.data) == std::get<bool>(b.data);
            case Value::Type::Array: {
                const auto& arrA = std::get<Value::Array>(a.data);
                const auto& arrB = std::get<Value::Array>(b.data);
                if (arrA->size() != arrB->size()) return false;
                for (size_t i = 0; i < arrA->size(); ++i) {
                    if (!areEqual((*arrA)[i], (*arrB)[i])) return false;
                }
                return true;
            }
            case Value::Type::Null:
                return true; // Both are null
            default:
                throw std::runtime_error("Equality comparison not supported");
        }
    }
}