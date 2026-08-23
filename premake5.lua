workspace "HouBackRender"
    architecture "x64"
    configurations {"Debug","Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Third = {}
Third["DearImgui"] = "%{prj.name}/Third/DearImgui"
Third["GLFW"] = "%{prj.name}/Third/GLFW"
Third["glad"] = "%{prj.name}/Third/glad"
Third["nlohmann_json"] = "%{prj.name}/Third/nlohmann_json"

project "HouBackRender"
    location "HouBackRender"
    -- kind "ConsoleApp"
    kind "WindowedApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")
    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

        "%{Third.glad}/src/glad.c",
        "%{Third.DearImgui}/*.h",
        "%{Third.DearImgui}/*.cpp",
        "%{Third.DearImgui}/backends/imgui_impl_glfw.h",
        "%{Third.DearImgui}/backends/imgui_impl_glfw.cpp",
        "%{Third.DearImgui}/backends/imgui_impl_opengl3.h",
        "%{Third.DearImgui}/backends/imgui_impl_opengl3.cpp",
        "%{Third.nlohmann_json}/json.hpp",
    }

    includedirs
    {
        "%{Third.glad}/include",
        "%{Third.GLFW}/include",
        "%{Third.DearImgui}",
        "%{Third.DearImgui}/backends",
        "%{Third.nlohmann_json}",
    }

    libdirs
    {
        "%{Third.GLFW}",
    }
    
    links
    {
        "opengl32.lib",
        "glfw3.lib",
    }
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"