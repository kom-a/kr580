workspace "KR580VM80A"
   configurations { "Debug", "Release" }
   architecture "x86"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Emulator"
    location "Emulator"
    kind "StaticLib"
    language "C++"

    targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.*",
        "%{prj.name}/%{prj.name}.h"
    }

    includedirs
    {
        "Shared/Include/",
        "Compiler/",
        "%{prj.name}/src/"
    }

    libdirs 
    {
        "Bin/" .. outputdir .. "/Compiler"
    }

    links
    {
        "Compiler"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Compiler"
    location "Compiler"
    kind "StaticLib"
    language "C++"

    targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.*",
        "%{prj.name}/%{prj.name}.h"
    }

    includedirs
    {
        "Shared/Include",
        "%{prj.name}/src"

    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "GLFW"
    location "GLFW"
    kind "StaticLib"
    language "C++"

    targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/null*.*",
        "%{prj.name}/src/context.*",
        "%{prj.name}/src/egl_context.*",
        "%{prj.name}/src/init.*",
        "%{prj.name}/src/input.*",
        "%{prj.name}/src/monitor.*",
        "%{prj.name}/src/osmesa_context.*",
        "%{prj.name}/src/vulkan.*",
        "%{prj.name}/src/wgl_context.*",
        "%{prj.name}/src/window.*",
        "%{prj.name}/src/platform.*",
    }

    includedirs
    {
        "%{prj.name}/include",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "system:Windows"
        defines { "_GLFW_WIN32 " }
        files "%{prj.name}/src/win32*.c"

    filter "system:Unix"
        defines { "_GLFW_X11 " }
        files "%{prj.name}/src/x11*.c"

    filter "system:Mac"
        defines { "_GLFW_COCOA " }
        files "%{prj.name}/src/cocoa*.c"

project "UMPK-80"
    location "UMPK-80"
    kind "ConsoleApp"
    language "C++"

    targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.*",
        "%{prj.name}/vendor/imgui/imgui/*.*",
        "%{prj.name}/vendor/glad/src/**.*",
        "%{prj.name}/vendor/imgui/imgui/backends/imgui_impl_glfw.*",
        "%{prj.name}/vendor/imgui/imgui/backends/imgui_impl_opengl3.*",
    }

    includedirs
    {
        "Compiler",
        "Emulator",
        "GLFW/include",
        "%{prj.name}/vendor/imgui",
        "%{prj.name}/vendor/imgui/imgui",
        "%{prj.name}/vendor/imgui/imgui/backends",
        "%{prj.name}/vendor/glad/include",
    }

    links
    {
        "GLFW"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
