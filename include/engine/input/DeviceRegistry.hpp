#pragma once

#include <string>
#include <string_view>

namespace ParteeEngine {
    using DeviceTypeID = uint16_t;

    class InputDevice;

    class DeviceRegistry {
    public:
        
        template<typename DeviceType>
        static constexpr DeviceTypeID typeID = computeId<DeviceType>();

    private:
        DeviceRegistry() = default;
        
        // static std::mutex mutex;

        // static std::unordered_map<DeviceTypeID, std::string> typeNames;

        template<typename T>
        static constexpr std::string_view getTypeName() {
            #if defined(__clang__)
                constexpr std::string_view name = __PRETTY_FUNCTION__;
                constexpr auto start = name.find("T = ") + 4;
                constexpr auto end = name.rfind(']');
                return name.substr(start, end - start);
            #elif defined(__GNUC__)
                constexpr std::string_view name = __PRETTY_FUNCTION__;
                constexpr auto start = name.find("T = ") + 4;
                constexpr auto end = name.rfind(';');
                return name.substr(start, end - start);
            #elif defined(_MSC_VER)
                constexpr std::string_view name = __FUNCSIG__;
                constexpr auto start = name.find("get<") + 4;
                constexpr auto end = name.rfind(">(");
                return name.substr(start, end - start);
            #else
                return "UnknownType";
            #endif
        }

        static constexpr uint32_t fnv1a_32(std::string_view str) {
            uint32_t hash = 2166136261u;  // FNV offset basis
            for (char c : str) {
                hash ^= static_cast<uint32_t>(c);
                hash *= 16777619u;  // FNV prime
            }
            return hash;
        }

        template<typename T>
        static constexpr DeviceTypeID computeId() {
            constexpr std::string_view name = getTypeName<T>();
            constexpr uint32_t hash = fnv1a_32(name);
            
            // Map to range [0, 65535]
            constexpr DeviceTypeID id = static_cast<DeviceTypeID>(
                (hash % 65535)
            );
            
            return id;
        }
    
    };

} // namespace ParteeEngine