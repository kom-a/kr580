workspace "KR580VM80A"
   configurations { "Debug", "Release" }
   architecture "x86"
   startproject "UMPK-80"

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
    location "UMPK-80/vendor/GLFW"
    kind "StaticLib"
    language "C++"

    targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
    objdir ("Bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.location}/src/null*.*",
        "%{prj.location}/src/context.*",
        "%{prj.location}/src/egl_context.*",
        "%{prj.location}/src/init.*",
        "%{prj.location}/src/input.*",
        "%{prj.location}/src/monitor.*",
        "%{prj.location}/src/osmesa_context.*",
        "%{prj.location}/src/vulkan.*",
        "%{prj.location}/src/wgl_context.*",
        "%{prj.location}/src/window.*",
        "%{prj.location}/src/platform.*",
    }

    includedirs
    {
        "%{prj.location}/include",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "system:Windows"
        defines { "_GLFW_WIN32 " }
        files "%{prj.location}/src/win32*.c"

    filter "system:Unix"
        defines { "_GLFW_X11 " }
        files "%{prj.location}/src/x11*.c"

    filter "system:Mac"
        defines { "_GLFW_COCOA " }
        files "%{prj.location}/src/cocoa*.c"

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
        "%{prj.name}/vendor/imgui/ImGuiColorTextEdit/*.cpp"
    }

    includedirs
    {
        "Compiler",
        "Emulator",
        "%{prj.name}/vendor/imgui",
        "%{prj.name}/vendor/imgui/imgui",
        "%{prj.name}/vendor/imgui/imgui/backends",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/GLFW/include",
    }

    links
    {
        "GLFW",
        "Emulator"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
