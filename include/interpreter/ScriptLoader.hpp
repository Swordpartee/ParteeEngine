#pragma once

#include <string>
#include <fstream>
#include <sstream>

namespace ParteeEngine {

    class ScriptLoader {
    public:
        /**
         * Load a script from a .par file
         * @param filePath Path to the .par script file
         * @return The contents of the file as a string
         * @throws std::runtime_error if file cannot be opened
         */
        static std::string loadScript(const std::string& filePath);

        /**
         * Load a script from a .par file and return success status
         * @param filePath Path to the .par script file
         * @param outContent Output parameter containing file contents
         * @return true if successful, false otherwise
         */
        static bool tryLoadScript(const std::string& filePath, std::string& outContent);
    };

} // namespace ParteeEngine
