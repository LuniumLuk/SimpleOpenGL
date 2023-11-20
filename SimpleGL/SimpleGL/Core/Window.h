#pragma once

#include "SimpleGL/Core/IO.h"

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
        bool fullscreen = false;
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

        uint32_t getWidth() const noexcept { return windowData.width; }
        uint32_t getHeight() const noexcept { return windowData.height; }

        void preframe() noexcept;
        bool update() noexcept;
        void endframe() noexcept;
        bool queryKeyPressed(int key) noexcept;
        bool queryMouseButtonPressed(int button) noexcept;
        void queryCursorPos(double* xpos, double* ypos) noexcept;
        void hideCursor(bool hide) noexcept;
    };

    /*
    * Keep track of the input state of the window
    * in order to avoid querying the state multiple times
    */
    struct InputState {

        bool rightButtonPressed = false;
        bool leftButtonPressed = false;
        float scrollDelta = 0.0f;
        float mouseDeltaX = 0.0f, mouseDeltaY = 0.0f;
        float mouseLastX = 0.0f, mouseLastY = 0.0f;

        bool keyPressed[KEY_LAST + 1] = { false };

        /*
        * Register only once, otherwise, the callbacks will be called multiple times
        */
        static void registerCallbacks(Window* window) noexcept;
        /*
        * Reset the delta values
        */
        static void resetDelta() noexcept;

        static InputState& instance() noexcept {
            static InputState state;
            return state;
        }

    public:
        InputState(InputState const&) = delete;
        InputState& operator=(InputState const&) = delete;

    private:
        InputState() = default;

    };

}
