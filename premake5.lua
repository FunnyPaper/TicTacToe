workspace "TicTacToe"
	architecture "x64"
	configurations { "Debug", "Release" }

outdir = "%{cfg.buildcfg}-%{cfg.system}"
project "TicTacToe"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("bin/" .. outdir)
	objdir("bin-obj/" .. outdir)
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"**glad.c"
	}
	
	removefiles 
	{ 
		"src/vendor/**.h",
		"src/vendor/**.cpp"
	}
	
	includedirs
	{
		"%{prj.location}/Dependencies/GLFW-3.3.2/include",
		"%{prj.location}/Dependencies/GLAD/include",
		"%{prj.location}/Dependencies/GLAD/src",
		"%{prj.location}/src/vendor/SOIL2/include",
		"%{prj.location}/src/vendor/GLM"
	}
	
	libdirs
	{
		"%{prj.location}/src/vendor/SOIL2/lib",
		"%{prj.location}/Dependencies/GLFW-3.3.2/lib"
	}
	
	links
	{
		"opengl32.lib",
		"glfw3.lib",
		"soil2-debug.lib"
	}
	
	filter "system:windows"
		cppdialect "C++14"
		systemversion "latest"
		
	filter "configurations:Debug"
		symbols "on"
		buildoptions "/MDd"
		
	filter "configurations:Release"
		optimize "on"