workspace "sixty-nine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "sixty-nine"
	location "sixty-nine"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/**.h",
		"%{prj.name}/vendor/**.hpp",
		"%{prj.name}/vendor/**.cpp",
		"%{prj.name}/vendor/**.c"
	}

	includedirs {
		"%{prj.name}/src",
		"Dependencies/ShockBaseLib/include",
		"Dependencies/ShockBaseLib/include/imgui",
		"Dependencies/ShockBaseLib/include/glad/include",
		"Dependencies/ShockBaseLib/include/glm/include",
		"Dependencies/spdlog/include",
		"Dependencies/GLFW/include",
		"Dependencies/asio",
		"Dependencies/olcNet"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SBL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SBL_DEBUG"
		symbols "On"

		libdirs {
			"Dependencies/ShockBaseLib/lib/Debug"
		}

		links {
			"ShockBaseLib"
		}

	filter "configurations:Release"
		defines "SBL_RELEASE"
		optimize "On"

		libdirs {
			"Dependencies/ShockBaseLib/lib/Release"
		}

		links {
			"ShockBaseLib"
		}

	filter "configurations:Dist"
		defines "SBL_DIST"
		optimize "On"

		libdirs {
			"Dependencies/ShockBaseLib/lib/Release"
		}

		links {
			"ShockBaseLib"
		}
