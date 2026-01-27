#include "engine/Window.hpp"

namespace ParteeEngine {
    Window::Window(const int width, const int height) : width(width), height(height) {
        const wchar_t CLASS_NAME[] = L"Partee Window Class";

        WNDCLASSW wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Added styles for OpenGL
        wc.hbrBackground = NULL;                       // No background for OpenGL

        RegisterClassW(&wc);

        // Store this pointer for the window
        hwnd = CreateWindowExW(
            0, CLASS_NAME, L"Partee Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, GetModuleHandle(NULL), this);

        if (hwnd == NULL) {
            return;
        }

        // Get device context
        hdc = GetDC(hwnd);

    };

    Window::~Window(){
        DestroyWindow(hwnd);
    }

    void Window::show() {
        ShowWindow(hwnd, SW_SHOW);
    }

    HWND Window::getHWND() const {
        return hwnd;
    }

    HDC Window::getHDC() const {
        return hdc;
    }

    bool Window::processMessages()
    {
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Window *pThis = nullptr;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
            pThis = (Window *)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        }
        else
        {
            pThis = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis)
        {
            switch (uMsg)
            {
            case WM_CLOSE:
                DestroyWindow(hwnd);
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}