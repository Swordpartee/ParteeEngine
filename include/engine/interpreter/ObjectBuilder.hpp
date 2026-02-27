#pragma once

#include "engine/interpreter/Interpreter.hpp"

namespace interpreter {

    inline auto makeNative = [](NativeFunction fn) {
        auto func = std::make_shared<ScriptFunction>();
        func->native = fn;
        return Value(func);
    };

    class ObjectBuilder {
    public:
        Value build() {
            auto obj = std::make_shared<Environment>();
            for (const auto& [key, value] : properties) {
                obj->define(key, value);
            }
            return Value(obj);
        }

        ObjectBuilder& addProperty(const std::string& key, const Value& value) {
            properties.emplace_back(key, value);
            return *this;
        }

        ObjectBuilder& addFunction(const std::string& key, const std::function<Value(std::vector<Value>)>& nativeFunc) {
            return addProperty(key, makeNative(nativeFunc));
        }

    private:
        std::vector<std::pair<std::string, Value>> properties;
    };

} // namespace interpreter