#include "PCH.h"

#include "SimpleGL/Core/Application.h"
#include "SimpleGL/Core/Timer.h"

namespace SGL {

    Application::Application(WindowOption const& opt) noexcept : window(opt) {

    }

    void Application::run() noexcept {
        init();
        while (!shouldExit) {
            window.preframe();
            if (!window.update())
                break;

            timer.update();

            double deltaTime = timer.getDeltaTime();
            accumulatedTime += deltaTime;
            while (accumulatedTime > fixedUpdateDelta) {
                fixedUpdate();
                accumulatedTime -= fixedUpdateDelta;
            }
            update(deltaTime);
            window.endframe();
        }
    }

    auto Application::terminate() noexcept -> void {
        shouldExit = true;
    }

}
