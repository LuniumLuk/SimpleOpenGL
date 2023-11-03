#pragma once

#include <chrono>

namespace SGL {

    struct Timer {

        std::chrono::steady_clock::time_point startTimePoint;
        std::chrono::steady_clock::time_point prevTimePoint;
        double _deltaTime = 0.0;

        Timer() {
            startTimePoint = std::chrono::steady_clock::now();
            prevTimePoint = startTimePoint;
        }

        void update() noexcept {
            auto now = std::chrono::steady_clock::now();
            auto deltaTimeCount = std::chrono::duration<double, std::micro>(now - prevTimePoint).count();
            _deltaTime = 0.000001 * deltaTimeCount;
            prevTimePoint = now;
        }

        double getDeltaTime() noexcept {
            return _deltaTime;
        }

        double getTotalTime() noexcept {
            auto totalTimeCount = std::chrono::duration<double, std::micro>(prevTimePoint - startTimePoint).count();
            return 0.000001 * totalTimeCount;
        }

    };
}