#include "engine/interpreter/ScriptLoader.hpp"
#include <iostream>
#include <filesystem>

namespace interpreter {

    std::string ScriptLoader::loadScript(const std::string& filePath) {
        // Construct a set of candidate paths to try
        std::vector<std::filesystem::path> candidates;
        std::filesystem::path requested(filePath);
        std::filesystem::path fileName = requested.filename();
        std::filesystem::path cwd = std::filesystem::current_path();

        // 1) As provided
        candidates.push_back(requested);
        // 2) Relative to CWD
        candidates.push_back(cwd / requested);
        // 3) In CWD/src
        candidates.push_back(cwd / "src" / fileName);

        // 4) Walk up to 6 parents and check the full path and just the filename
        std::filesystem::path cur = cwd;
        for (int i = 0; i < 6; ++i) {
            // Check the full relative path (e.g., assets/scripts/exampleCode.par)
            candidates.push_back(cur / requested);
            // Check just the filename
            candidates.push_back(cur / fileName);
            // Check in src subdirectory
            candidates.push_back(cur / "src" / fileName);
            if (cur.has_parent_path()) {
                cur = cur.parent_path();
            } else {
                break;
            }
        }

        // Try each candidate
        for (const auto& candidate : candidates) {
            try {
                if (std::filesystem::exists(candidate) && std::filesystem::is_regular_file(candidate)) {
                    std::ifstream file(candidate.string());
                    if (file.is_open()) {
                        std::stringstream buffer;
                        buffer << file.rdbuf();
                        file.close();
                        return buffer.str();
                    }
                }
            } catch (...) {
                // ignore and continue
            }
        }

        throw std::runtime_error("Failed to open script file: " + filePath);
    }

} // namespace interpreter
