project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/glfw_config.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c",
    }

    filter "system:linux"
        systemversion "latest"
        pic "on"

        files
        {
            "src/x11_platform.h",
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/xkb_unicode.c",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/posix_module.c",
            "src/glx_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c",
            "src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:macosx"
        pic "on"

        files
        {
            "src/cocoa_platform.h",
            "src/cocoa_init.m",
            "src/cocoa_monitor.m",
            "src/cocoa_window.m",
            "src/cocoa_joystick.m",
            "src/cocoa_time.c",
            "src/nsgl_context.m",
            "src/posix_thread.c",
            "src/posix_module.c",
            "src/osmesa_context.c",
            "src/egl_context.c"
        }

        defines
        {
            "_GLFW_COCOA"
        }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "src/win32_platform.h",
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"