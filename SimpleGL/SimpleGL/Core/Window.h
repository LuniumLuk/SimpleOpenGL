#pragma once

namespace SGL {

    typedef void (*FramebufferSizeCallback)(int width, int height);
    typedef void (*KeyCallback)(int key, int action);
    typedef void (*MouseButtonCallback)(int button, int action);
    typedef void (*ScrollCallback)(double xoffset, double yoffset);
    typedef void (*CursorPosCallback)(double xpos, double ypos);

    struct WindowOption {
        std::string name = "SimpleGL";
        uint32_t width = 720;
        uint32_t height = 720;
        bool fullScreen = false;
        bool vsync = true;
        bool debug = true;
        bool resizable = true;
        bool imgui = true;
        int sampling = 1;
    };

    struct Window {

        struct WindowData {
            uint32_t width = 0;
            uint32_t height = 0;
            std::vector<FramebufferSizeCallback> framebufferSizeCallbacks;
            std::vector<KeyCallback> keyCallbacks;
            std::vector<MouseButtonCallback> mouseButtonCallbacks;
            std::vector<ScrollCallback> scrollCallbacks;
            std::vector<CursorPosCallback> cursorPosCallbacks;
        };

        void* handle = nullptr;
        WindowData windowData;

        Window() = delete;
        Window(WindowOption const& opt);
        ~Window();

        Window& operator=(Window const&) = delete;
        Window(Window const&) = delete;

        void preframe() noexcept;
        bool update() noexcept;
        void endframe() noexcept;
        bool queryKeyPressed(int key) noexcept;
        bool queryMouseButtonPressed(int button) noexcept;
        void queryCursorPos(double* xpos, double* ypos) noexcept;
        void hideCursor(bool hide) noexcept;
    };

}
