#pragma once

#include <functional>
#include <memory>

namespace parteeengine::rendering {
    
    // Configuration for window creation
    struct WindowConfig {
        int width = 800;
        int height = 600;
        const char* title = "ParteeEngine Window";
    };

    // Window event types
    struct WindowEvent {
        enum class Type {
            Close,    // User requested window closure
            Resize    // Window was resized
        };

        Type type;
        explicit WindowEvent(Type t) : type(t) {}
    };

    // Platform-agnostic graphics context handle. Contains native windowing
    // and graphics handles for the current platform (Windows, Linux, macOS, etc.)
    struct NativeGraphicsContext {
        void* deviceContext = nullptr;   // HDC on Windows, Display* on X11, etc.
        void* renderContext = nullptr;   // HGLRC on Windows, GLXContext on X11, etc.
        void* windowHandle = nullptr;    // HWND on Windows, Window on X11, etc.
    };

    using WindowEventCallback = std::function<void(const WindowEvent&)>;

    // Platform-agnostic window interface. Implementations should provide
    // native window creation, event handling, and graphics context management.
    class IWindow {
    public:
        virtual ~IWindow() = default;

        virtual bool create(const WindowConfig& config) = 0;
        virtual bool create() = 0;
        virtual bool destroy() = 0;

        virtual void setConfig(const WindowConfig& config) = 0;
        virtual bool show() = 0;
        virtual bool hide() = 0;
        
        virtual bool swapBuffers() = 0;
        virtual bool pollEvents() = 0;

        virtual NativeGraphicsContext getNativeContext() const = 0;
        virtual WindowConfig getConfig() const = 0;

        virtual void setEventCallback(WindowEventCallback callback) = 0;

        // Factory method. Returns platform-specific implementation (W32Window on Windows).
        static std::unique_ptr<IWindow> createPlatformWindow();
        
    protected:
        IWindow() = default;
    };

} // namespace parteeengine::rendering