#include "PCH.h"

#include "SimpleGL/Core/IO.h"
#include "SimpleGL/Core/Window.h"
#include "SimpleGL/Core/ImGuiHelper.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace SGL {

    void InputState::registerCallbacks(Window* window) noexcept {

        window->windowData.keyCallbacks.push_back(
            [](int key, int action) {
                InputState::instance().keyPressed[key] = (action != KEY_RELEASED);
            });

        window->windowData.mouseButtonCallbacks.push_back(
            [](int button, int action) {
                if (action == BUTTON_PRESSED) {
                    if (button == BUTTON_RIGHT) {
                        InputState::instance().rightButtonPressed = true;
                    }
                    else if (button == BUTTON_LEFT) {
                        InputState::instance().leftButtonPressed = true;
                    }
                }
                else if (action == BUTTON_RELEASED) {
                    if (button == BUTTON_RIGHT) {
                        InputState::instance().rightButtonPressed = false;
                    }
                    else if (button == BUTTON_LEFT) {
                        InputState::instance().leftButtonPressed = false;
                    }
                }
            });

        window->windowData.scrollCallbacks.push_back(
            [](double xoffset, double yoffset) {
                InputState::instance().scrollDelta = (float)yoffset;
            });

        window->windowData.cursorPosCallbacks.push_back(
            [](double xpos, double ypos) {
                InputState::instance().mouseDeltaX = (float)xpos - InputState::instance().mouseLastX;
                InputState::instance().mouseDeltaY = (float)ypos - InputState::instance().mouseLastY;
                InputState::instance().mouseLastX = (float)xpos;
                InputState::instance().mouseLastY = (float)ypos;
            });

    }

    void InputState::resetDelta() noexcept {
        InputState::instance().scrollDelta = 0.0f;
        InputState::instance().mouseDeltaX = 0.0f;
        InputState::instance().mouseDeltaY = 0.0f;
    }

    Window::Window(WindowOption const& opt) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, opt.resizable);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, opt.debug);
        glfwWindowHint(GLFW_SAMPLES, opt.sampling);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int windowWidth = opt.width;
        int windowHeight = opt.height;

        if (opt.fullscreen) {
            glfwWindowHint(GLFW_DECORATED, false);
            windowWidth = screenWidth;
            windowHeight = screenHeight;
        }

        GLFWwindow* glfwHandle = glfwCreateWindow(windowWidth, windowHeight, opt.name.c_str(), nullptr, nullptr);
        if (glfwHandle == nullptr) {
            SGL_LOG_FATAL("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwSetWindowUserPointer(glfwHandle, &windowData);

        SGL_LOG_INFO("Initilizing GLFW window, width: {0}, height: {1}", windowWidth, windowHeight);
        if (opt.fullscreen) {
            windowData.width = GetSystemMetrics(SM_CXSCREEN);
            windowData.height = GetSystemMetrics(SM_CYSCREEN);
            glfwSetWindowPos(glfwHandle, 0, 0);
        }
        else {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            glfwSetWindowPos(glfwHandle, screenWidth / 2 - windowWidth / 2, screenHeight / 2 - windowHeight / 2);
            windowData.width = windowWidth;
            windowData.height = windowHeight;
        }

        glfwSwapInterval(opt.vsync ? 1 : 0);
        glfwMakeContextCurrent(glfwHandle);

        glfwSetFramebufferSizeCallback(glfwHandle,
            [](GLFWwindow* window, int width, int height) {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data->width = width;
                data->height = height;
                for (auto& callback : data->framebufferSizeCallbacks) {
                    callback(width, height);
                }
            });

        glfwSetKeyCallback(glfwHandle,
            [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                for (auto& callback : data->keyCallbacks) {
                    callback(key, action);
                }
            });

        glfwSetMouseButtonCallback(glfwHandle,
            [](GLFWwindow* window, int button, int action, int mods) {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                for (auto& callback : data->mouseButtonCallbacks) {
                    callback(button, action);
                }
            });

        glfwSetScrollCallback(glfwHandle,
            [](GLFWwindow* window, double xoffset, double yoffset) {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                for (auto& callback : data->scrollCallbacks) {
                    callback(xoffset, yoffset);
                }
            });

        glfwSetCursorPosCallback(glfwHandle,
            [](GLFWwindow* window, double xpos, double ypos) {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                for (auto& callback : data->cursorPosCallbacks) {
                    callback(xpos, ypos);
                }
            });

        gladLoadGL();

        if (opt.imgui) {
            SGL_LOG_INFO("Initilizing ImGui");
            initImGui(glfwHandle);
        }

        if (opt.debug) {
            SGL_LOG_INFO("Initilizing OpenGL debugging");
            int flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
                    // ignore non-significant error/warning codes
                    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

                    SGL_LOG_DEBUG("{0}: {1}", id, message);

                    switch (source) {
                    case GL_DEBUG_SOURCE_API:               SGL_LOG_DEBUG("Source: API"); break;
                    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     SGL_LOG_DEBUG("Source: Window System"); break;
                    case GL_DEBUG_SOURCE_SHADER_COMPILER:   SGL_LOG_DEBUG("Source: Shader Compiler"); break;
                    case GL_DEBUG_SOURCE_THIRD_PARTY:       SGL_LOG_DEBUG("Source: Third Party"); break;
                    case GL_DEBUG_SOURCE_APPLICATION:       SGL_LOG_DEBUG("Source: Application"); break;
                    case GL_DEBUG_SOURCE_OTHER:             SGL_LOG_DEBUG("Source: Other"); break;
                    }

                    switch (type) {
                    case GL_DEBUG_TYPE_ERROR:               SGL_LOG_DEBUG("Type: Error"); break;
                    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: SGL_LOG_DEBUG("Type: Deprecated Behaviour"); break;
                    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  SGL_LOG_DEBUG("Type: Undefined Behaviour"); break;
                    case GL_DEBUG_TYPE_PORTABILITY:         SGL_LOG_DEBUG("Type: Portability"); break;
                    case GL_DEBUG_TYPE_PERFORMANCE:         SGL_LOG_DEBUG("Type: Performance"); break;
                    case GL_DEBUG_TYPE_MARKER:              SGL_LOG_DEBUG("Type: Marker"); break;
                    case GL_DEBUG_TYPE_PUSH_GROUP:          SGL_LOG_DEBUG("Type: Push Group"); break;
                    case GL_DEBUG_TYPE_POP_GROUP:           SGL_LOG_DEBUG("Type: Pop Group"); break;
                    case GL_DEBUG_TYPE_OTHER:               SGL_LOG_DEBUG("Type: Other"); break;
                    }

                    switch (severity) {
                    case GL_DEBUG_SEVERITY_HIGH:            SGL_LOG_DEBUG("Severity: high"); break;
                    case GL_DEBUG_SEVERITY_MEDIUM:          SGL_LOG_DEBUG("Severity: medium"); break;
                    case GL_DEBUG_SEVERITY_LOW:             SGL_LOG_DEBUG("Severity: low"); break;
                    case GL_DEBUG_SEVERITY_NOTIFICATION:    SGL_LOG_DEBUG("Severity: notification"); break;
                    }
                }, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                glDebugMessageControl(GL_DEBUG_SOURCE_API,
                    GL_DEBUG_TYPE_ERROR,
                    GL_DEBUG_SEVERITY_HIGH,
                    0, nullptr, GL_TRUE);
            }
        }

        handle = (void*)glfwHandle;

        InputState::instance().registerCallbacks(this);
    }

    Window::~Window() {
        shutdownImGui();
        glfwTerminate();
    }

    void Window::preframe() noexcept {
        preframeImGui();
    }

    bool Window::update() noexcept {
        if (glfwWindowShouldClose((GLFWwindow*)handle)) return false;
        return true;
    }

    void Window::endframe() noexcept {
        endframeImGui();
        InputState::instance().resetDelta();
        glfwSwapBuffers((GLFWwindow*)handle);
        glfwPollEvents();
    }

    bool Window::queryKeyPressed(int key) noexcept {
        return glfwGetKey((GLFWwindow*)handle, key) == GLFW_PRESS;
    }

    bool Window::queryMouseButtonPressed(int button) noexcept {
        return glfwGetMouseButton((GLFWwindow*)handle, button) == GLFW_PRESS;
    }

    void Window::queryCursorPos(double* xpos, double* ypos) noexcept {
        glfwGetCursorPos((GLFWwindow*)handle, xpos, ypos);
    }

    void Window::hideCursor(bool hide) noexcept {
        glfwSetInputMode((GLFWwindow*)handle, GLFW_CURSOR, hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

}
