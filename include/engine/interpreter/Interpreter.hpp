#pragma once

#include "interpreter/Lexer.hpp"
#include "interpreter/Parser.hpp"
#include "interpreter/AST.hpp"
#include "interpreter/ScriptLoader.hpp"

#include <variant>
#include <unordered_map>
#include <functional>
#include <memory>

namespace interpreter {

    struct Engine;
    struct Environment;
    struct Value;
    
    using NativeFunction = std::function<Value(std::vector<Value>)>;

    struct ScriptFunction {
        std::vector<std::string> params;
        Expr* body;
        std::shared_ptr<Environment> closure;
        NativeFunction native; // if set, call this instead of body
    };

    struct Value {
        enum class Type {
            Number,
            String,
            Boolean,
            Array,
            Function,
            Object,
            Null
        } type = Type::Null;
        using Array = std::shared_ptr<std::vector<Value>>;
        using Function = std::shared_ptr<ScriptFunction>;
        using Object = std::shared_ptr<Environment>;

        std::variant<std::monostate, double, std::string, bool, Array, Function, Object> data;

        Value() : type(Type::Null), data(std::monostate{}) {}
        Value(double num) : type(Type::Number), data(num) {}
        Value(const std::string& str) : type(Type::String), data(str) {}
        Value(bool boolean) : type(Type::Boolean), data(boolean) {}
        Value(const Array& arr) : type(Type::Array), data(arr) {}
        Value(const Function& func) : type(Type::Function), data(func) {}
        Value(const Object& obj) : type(Type::Object), data(obj) {}
        Value(std::nullptr_t) : type(Type::Null), data(std::monostate{}) {}
    };

    struct Environment : public std::enable_shared_from_this<Environment> {
        std::unordered_map<std::string, Value> variables;
        std::shared_ptr<Environment> parent = nullptr;

        static std::shared_ptr<Environment> make(std::shared_ptr<Environment> parent = nullptr) {
            auto env = std::make_shared<Environment>();
            env->parent = parent;
            return env;
        }

        void define(const std::string& name, const Value& value);
        void set(const std::string& name, const Value& value);
        Value get(const std::string& name);
        bool has(const std::string& name) const;
    };

     // Control flow signals as exceptions
    class BreakSignal : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Break signal";
        }
    };

    class ContinueSignal : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Continue signal";
        }
    };

    class ReturnSignal : public std::exception {
    public:
        Value returnValue;
        
        explicit ReturnSignal(const Value& val);
        
        const char* what() const noexcept override {
            return "Return signal";
        }
    };

    class Interpreter {
    public:
        Interpreter() = default;

        void interpret(const std::string& source, Engine* engine);

    private:
        std::shared_ptr<Environment> globalEnv = Environment::make();
        Engine* engine = nullptr; // Pointer to the game engine for integration

        void registerGlobals();
        void execute(Program* program);
        
        // Main dispatcher - routes to appropriate typed overload
        Value execute(Expr* expr, Environment* env);
        
        // Typed overloads for each expression type
        Value execute(ErrorExpr* expr, Environment* env);
        Value execute(NumberLiteral* expr, Environment* env);
        Value execute(StringLiteral* expr, Environment* env);
        Value execute(BooleanLiteral* expr, Environment* env);
        Value execute(Identifier* expr, Environment* env);
        Value execute(ArrayLiteral* expr, Environment* env);
        Value execute(EventLiteral* expr, Environment* env);
        Value execute(UnaryExpr* expr, Environment* env);
        Value execute(BinaryExpr* expr, Environment* env);
        Value execute(BodyExpr* expr, Environment* env);
        Value execute(UseExpr* expr, Environment* env);
        Value execute(OnExpr* expr, Environment* env);
        Value execute(ReturnExpr* expr, Environment* env);
        Value execute(ContinueExpr* expr, Environment* env);
        Value execute(BreakExpr* expr, Environment* env);
        Value execute(CaseExpr* expr, Environment* env);
        Value execute(DefaultExpr* expr, Environment* env);
        Value execute(SwitchExpr* expr, Environment* env);
        Value execute(IfExpr* expr, Environment* env);
        Value execute(WhileExpr* expr, Environment* env);
        Value execute(ForExpr* expr, Environment* env);
        Value execute(FunctionExpr* expr, Environment* env);
        Value execute(CallExpr* expr, Environment* env);
        Value execute(IndexExpr* expr, Environment* env);

        bool areEqual(const Value& left, const Value& right);
    };
} // namespace interpreter