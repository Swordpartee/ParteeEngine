#pragma once

#include "engine/rendering/window/IWindow.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace parteeengine::rendering {

    // Windows 10+ native window implementation using Win32 API.
    // Manages HWND, device context, and OpenGL rendering context.
    class W32Window : public IWindow {
    public:
        W32Window();
        ~W32Window() override;

        bool create(const WindowConfig& config) override;
        bool create() override;
        bool destroy() override;
        
        void setConfig(const WindowConfig& config) override;
        bool show() override;
        bool hide() override;

        bool swapBuffers() override;
        bool pollEvents() override;

        NativeGraphicsContext getNativeContext() const override;
        WindowConfig getConfig() const override;

        void setEventCallback(WindowEventCallback callback) override;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

        HWND hwnd = nullptr;
        HDC hdc = nullptr;
        HGLRC hglrc = nullptr;

        WindowConfig config = {};
        WindowEventCallback eventCallback;
    };

} // namespace parteeengine::rendering