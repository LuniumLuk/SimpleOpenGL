#pragma once

namespace SGL {

    constexpr double PI() noexcept { return 3.14159265; }
    constexpr double PI_mul_two() noexcept { return 3.14159265 * 2; }
    constexpr double PI_div_two() noexcept { return 3.14159265 / 2; }
    constexpr double PI_div_four() noexcept { return 3.14159265 / 4; }
    constexpr double one_div_PI() noexcept { return 1.0 / 3.14159265; }

    constexpr double Deg2Rad(double deg) { return deg / 180.0 * PI(); }
    constexpr double Rad2Deg(double rad) { return rad / PI() * 180.0; }

}
