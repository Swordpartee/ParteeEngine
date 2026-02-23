#pragma once

#include "engine/rendering/windows/IWindow.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace parteeengine::rendering {

    class W32Window : public IWindow {
    public:
        W32Window();
        ~W32Window() override;

        bool create(const WindowConfig& config) override;
        bool create() override;
        bool destroy() override;
        bool show() override;
        bool hide() override;

        bool swapBuffers() override;
        bool pollEvents() override;

        NativeGraphicsContext getNativeContext() const override;
        WindowConfig getConfig() const override;
        void config(WindowConfig config);

        // void setEventCallback(WindowEventCallback callback) override;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

        HWND hwnd = nullptr;
        HDC hdc = nullptr;
        
        WindowConfig windowConfig = {};

        // WindowEventCallback eventCallback;

    };

} // namespace parteeengine::rendering