#include "engine/rendering/windows/IWindow.hpp"

#include "engine/rendering/windows/W32Window.hpp"

namespace parteeengine::rendering {
    std::unique_ptr<IWindow> IWindow::createPlatformWindow() {
        #if defined(_WIN32)
            return std::make_unique<W32Window>();
        #elif defined(__linux__)
            // return std::make_unique<X11Window>();
            static_assert(false, "Linux window not yet implemented");
        #elif defined(__APPLE__)
            // return std::make_unique<CocoaWindow>();
            static_assert(false, "macOS window not yet implemented");
        #else
            static_assert(false, "Unsupported platform");
        #endif
    }
} // namespace parteeengine::rendering