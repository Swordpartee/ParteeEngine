#pragma once

#include "IWindow.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace ParteeEngine::Rendering {

    class W32Window : public IWindow {
    public:
        W32Window();
        ~W32Window() override;

        bool create(const WindowConfig& config) override;
        bool destroy() override;
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

} // namespace ParteeEngine