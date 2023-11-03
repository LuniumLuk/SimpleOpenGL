workspace "SimpleGL"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    enginedir = path.getabsolute("%{prj.name}")

    includedir = {}
    includedir["GLFW"] = "Vendor/glfw/include"
    includedir["Glad"] = "Vendor/glad/include"
    includedir["ImGui"] = "Vendor/imgui"
    includedir["glm"] = "Vendor/glm"
    includedir["assimp"] = "Vendor/assimp/include"

    group "Dependencies"

        include "Vendor/glfw"
        include "Vendor/glad"
        include "Vendor/ImGui"
        include "Vendor/assimp"

    group ""

    project "SimpleGL"
        location "SimpleGL"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{wks.location}/Bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "PCH.h"
        pchsource "SimpleGL/PCH.cpp"

        files
        {
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp"
        }

        includedirs
        {
            "%{prj.name}",
            "Vendor",
            "%{includedir.GLFW}",
            "%{includedir.Glad}",
            "%{includedir.ImGui}",
            "%{includedir.glm}",
            "%{includedir.assimp}",
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "assimp",
            "opengl32.lib",
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SGL_ENABLE_ASSERTS",
                "GLFW_INCLUDE_NONE",
                "_CRT_SECURE_NO_WARNINGS",
            }

        filter "configurations:Debug"
            defines "SGL_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "SGL_RELEASE"
            runtime "Release"
            optimize "on"

    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{wks.location}/Bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp"
        }

        includedirs
        {
            "SimpleGL",
            "%{prj.name}",
            "Vendor",
            "%{includedir.GLFW}",
            "%{includedir.Glad}",
            "%{includedir.ImGui}",
            "%{includedir.glm}",
        }

        links
        {
            "SimpleGL"
        }

        filter "system:windows"
            systemversion "latest"
            defines
            {
                "SGL_ENABLE_ASSERTS"
            }

        filter "configurations:Debug"
            defines "SGL_DEBUG"
            symbols "on"

        filter "configurations:Release"
            defines "SGL_RELEASE"
            optimize "on"
