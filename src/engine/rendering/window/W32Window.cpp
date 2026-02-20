#include "engine/rendering/window/W32Window.hpp"

namespace parteeengine::rendering {

    W32Window::W32Window() = default;

    W32Window::~W32Window() {
        destroy();
    }

    bool W32Window::create(const WindowConfig& windowConfig) {
        config = windowConfig;
        return create();
    }

    bool W32Window::create() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = W32Window::WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = "ParteeEngineWindowClass";

        RegisterClassEx(&wc);

        DWORD stype = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

        RECT rect = {0, 0, config.width, config.height};
        AdjustWindowRect(&rect, stype, FALSE);

        hwnd = CreateWindowEx(
            0, // Optional window styles
            wc.lpszClassName, // Window class
            config.title, // Window title
            stype,  // Window style
            CW_USEDEFAULT, CW_USEDEFAULT, // Size and position
            rect.right - rect.left, // Width
            rect.bottom - rect.top, // Height
            nullptr, // Parent window
            nullptr, // Menu
            wc.hInstance, // Instance handle
            this // Additional application data
        );

        if (!hwnd) return false;

        hdc = GetDC(hwnd);
        return hdc != nullptr;
    }

    bool W32Window::destroy() {
        if (hglrc) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
            hglrc = nullptr;
        }
        if (hdc && hwnd) {
            ReleaseDC(hwnd, hdc);
            hdc = nullptr;
        }
        if (hwnd) {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
        return true;
    }

    void W32Window::setConfig(const WindowConfig& newConfig) {
        config = newConfig;
        
        if (hwnd) {
            UpdateWindow(hwnd);
        }
    }
    

    bool W32Window::show() {
        ShowWindow(hwnd, SW_SHOW);
        return UpdateWindow(hwnd);
    }

    bool W32Window::hide() {
        return ShowWindow(hwnd, SW_HIDE);
    }
    
    bool W32Window::swapBuffers() {
        return SwapBuffers(hdc);
    }

    bool W32Window::pollEvents() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

    NativeGraphicsContext W32Window::getNativeContext() const {
        return NativeGraphicsContext{hdc, hglrc, hwnd};
    }

    LRESULT CALLBACK W32Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        W32Window* window = nullptr;
        if (msg == WM_NCCREATE) {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            window = reinterpret_cast<W32Window*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        } else {
            window = reinterpret_cast<W32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (window) {
            window->handleMessage(msg, wParam, lParam);
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void W32Window::handleMessage(UINT msg, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam) {
        if (eventCallback) {
            switch (msg) {
                case WM_CLOSE:
                    eventCallback(WindowEvent{WindowEvent::Type::Close});
                    break;
                case WM_SIZE:
                    eventCallback(WindowEvent{WindowEvent::Type::Resize});
                    break;
                default:
                    break;
            }
        }
    }

    WindowConfig W32Window::getConfig() const {
        return config;
    }

    void W32Window::setEventCallback(WindowEventCallback callback) {
        eventCallback = callback;
    }

} // namespace parteeengine::rendering