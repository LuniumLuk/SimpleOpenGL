#include "PCH.h"

#include "SimpleGL/Core/Application.h"
#include "SimpleGL/Core/Timer.h"

namespace SGL {

    void Application::createWindow(WindowOption const& opt) noexcept {
        if (window) {
            SGL_LOG_WARN("Window already initialized");
            return;
        }
        window = new Window(opt);
    }

    void Application::run() noexcept {
        init();
        if (!window) {
            SGL_LOG_WARN("Window not initialized, using default configuration");
            window = new Window(WindowOption());
        }
        while (!shouldExit) {
            window->preframe();
            if (!window->update())
                break;

            timer.update();

            double deltaTime = timer.getDeltaTime();
            accumulatedTime += deltaTime;
            while (accumulatedTime > fixedUpdateDelta) {
                fixedUpdate();
                accumulatedTime -= fixedUpdateDelta;
            }
            update(deltaTime);
            window->endframe();
        }
    }

    auto Application::terminate() noexcept -> void {
        shouldExit = true;
    }

}
