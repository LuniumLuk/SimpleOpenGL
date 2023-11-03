#pragma once

#include "SimpleGL/Core/Window.h"
#include "SimpleGL/Core/Timer.h"

namespace SGL {

    struct Application {

        Application(WindowOption const& opt) noexcept;

        double const fixedUpdateDelta = 0.016;
        double accumulatedTime = 0.0;
        bool shouldExit = false;
        Window window;
        Timer timer;

        void run() noexcept;
        void terminate() noexcept;

        virtual void init() noexcept = 0;
        virtual void update(double deltaTime) noexcept = 0;
        virtual void fixedUpdate() noexcept = 0;

    };

}
