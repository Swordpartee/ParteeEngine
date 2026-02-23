#pragma once

#include <functional>
#include <memory>

namespace parteeengine::rendering {
    
    struct WindowConfig {
        int width = 800;
        int height = 600;
        const char* title = "ParteeEngine Window";
    };

    // struct WindowEvent : public Event {
    //     enum class Type {
    //         Close,
    //         Resize
    //     };

    //     void subscribed() override {};

    //     Type type;

    //     explicit WindowEvent(Type t) : type(t) {}
    // };

    // Platform-agnostic graphics context handle
    struct NativeGraphicsContext {
        void* deviceContext = nullptr;   // HDC on Windows, Display* on X11
        void* windowHandle = nullptr;    // HWND on Windows, Window on X11
    };

    // using WindowEventCallback = std::function<void(const WindowEvent&)>;

    class IWindow {
    public:
        virtual ~IWindow() = default;

        virtual bool create(const WindowConfig& config) = 0;
        virtual bool create() = 0;
        virtual bool destroy() = 0;
        virtual bool show() = 0;
        virtual bool hide() = 0;
        
        virtual bool swapBuffers() = 0;
        virtual bool pollEvents() = 0;

        virtual NativeGraphicsContext getNativeContext() const = 0;
        virtual WindowConfig getConfig() const = 0;
        virtual void config(WindowConfig config) = 0;

        // virtual void setEventCallback(WindowEventCallback callback) = 0;

        static std::unique_ptr<IWindow> createPlatformWindow();
        
    protected:
        IWindow() = default;
    };

} // namespace parteeengine::rendering