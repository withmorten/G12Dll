newoption {
	trigger     = "no-opt",
	description = "disable optimization for release builds"
}

newoption {
	trigger     = "no-full-paths",
	description = "don't print full paths into binary"
}

workspace "G12Dll"
	function setdebugpath (gamepath, exename, args)
		debugdir (gamepath)
		debugcommand (gamepath .. "/" .. exename)
		debugargs (args)
	end

	function addpostbuildcopy(gamepath)
		postbuildcommands { '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. gamepath .. "/" .. '%{cfg.buildtarget.name}"' }
	end

	configurations
	{
		"Debug",
		"Release",
	}

	startproject "DInput"

	location "build"

	characterset ("MBCS")
	toolset ("v141_xp")
	floatingpoint "fast"

	links { "legacy_stdio_definitions" }

	if (_OPTIONS["no-full-paths"]) then
		linkoptions "/PDBALTPATH:%_PDB%"
	end

	defines { "_WINDOWS", "WIN32", "WIN32_LEAN_AND_MEAN", "VC_EXTRALEAN", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "_CRT_NONSTDC_NO_WARNINGS", "_USE_32BIT_TIME_T", "NOMINMAX" }

	buildoptions { "/Zc:sizedDealloc-" }
	removeunreferencedcodedata "off"
	randomizedbaseaddress "off"
	imagehassafeexceptionhandlers "off"
	vectorextensions "IA32"
	stringpooling "on"
	staticruntime "on"
	justmycode "off"

	if (_OPTIONS["no-full-paths"]) then
		usefullpaths "off"
	end

	vpaths { ["Header Files"] = { "**.h", "**.hpp" } }
	vpaths { ["Resource Files"] = { "**.rc", "**.def" } }
	vpaths { ["Source Files"] = { "**.c", "**.cpp" } }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "full"
		optimize "off"
		runtime "debug"
		editAndContinue "off"
		flags { "NoIncrementalLink" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "release"
		symbols "on"
		enablecomdatfolding "off"
		if (_OPTIONS["no-opt"]) then
			symbols "default"
			optimize "off"
			functionlevellinking "on"
			optimizereferences "off"
			flags { "NoIncrementalLink" }
		else
			optimize "speed"
			inlining "auto"
			vectorextensions "SSE2"
			flags { "LinkTimeOptimization" }
			flags { "NoBufferSecurityCheck" }
			defines { "LTCG" }
			omitframepointer "on"
		end

project "DInput"
	kind "SharedLib"
	language "C++"
	targetname "dinput"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12Core.h" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "DInput/resource.h" }
	files { "DInput/DInput.rc" }

	files { "DInput/dinput.cpp" }
	files { "DInput/main.def" }

	addpostbuildcopy(os.getenv("GOTHIC101D_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC101E_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC104D_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC107C_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC112F_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC26_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC12_DIR"))

project "G12Dll"
	kind "SharedLib"
	language "C++"
	targetname "g12"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G12Dll/resource.h" }
	files { "G12Dll/G12Dll.rc" }

	files { "G12Dll/g12.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC101D_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC101E_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC104D_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC107C_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC112F_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC26_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC12_DIR"))

project "G101Fixes"
	kind "SharedLib"
	language "C++"
	targetname "g101fixes"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G101.h" }
	files { "G12Dll/G101.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G101Fixes/resource.h" }
	files { "G101Fixes/G101Fixes.rc" }

	files { "G101Fixes/g101fixes.hpp" }
	files { "G101Fixes/g101fixes.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC101D_DIR"))
	setdebugpath(os.getenv("GOTHIC101D_DIR"), "gothic.exe", "")

project "G1Barrier"
	kind "SharedLib"
	language "C++"
	targetname "g1barrier"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G1.h" }
	files { "G12Dll/G1.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G1Barrier/resource.h" }
	files { "G1Barrier/G1Barrier.rc" }

	files { "G1Barrier/g1barrier.hpp" }
	files { "G1Barrier/g1barrier.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	setdebugpath(os.getenv("GOTHIC108K_DIR"), "Gothic.exe", "")

project "G1Fixes"
	kind "SharedLib"
	language "C++"
	targetname "g1fixes"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G1.h" }
	files { "G12Dll/G1.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G1Fixes/resource.h" }
	files { "G1Fixes/G1Fixes.rc" }

	files { "G1Fixes/g1fixes.hpp" }
	files { "G1Fixes/g1fixes.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	setdebugpath(os.getenv("GOTHIC108K_DIR"), "Gothic.exe", "")

project "G1Spells"
	kind "SharedLib"
	language "C++"
	targetname "g1spells"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G1.h" }
	files { "G12Dll/G1.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G1Spells/resource.h" }
	files { "G1Spells/G1Spells.rc" }

	files { "G1Spells/g1spells.hpp" }
	files { "G1Spells/g1spells.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	setdebugpath(os.getenv("GOTHIC108K_DIR"), "Gothic.exe", "")

project "G2Barrier"
	kind "SharedLib"
	language "C++"
	targetname "g2barrier"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G2.h" }
	files { "G12Dll/G2.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G2Barrier/resource.h" }
	files { "G2Barrier/G2Barrier.rc" }

	files { "G2Barrier/g2barrier.hpp" }
	files { "G2Barrier/g2barrier.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC26_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC12_DIR"))
	setdebugpath(os.getenv("GOTHIC12_DIR"), "Gothic2.exe", "")

project "G2Fixes"
	kind "SharedLib"
	language "C++"
	targetname "g2fixes"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G2.h" }
	files { "G12Dll/G2.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G2Fixes/resource.h" }
	files { "G2Fixes/G2Fixes.rc" }

	files { "G2Fixes/g2fixes.hpp" }
	files { "G2Fixes/g2fixes.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC26_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC12_DIR"))
	setdebugpath(os.getenv("GOTHIC12_DIR"), "Gothic2.exe", "")

project "G2Spells"
	kind "SharedLib"
	language "C++"
	targetname "g2spells"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/G2.h" }
	files { "G12Dll/G2.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "G2Spells/resource.h" }
	files { "G2Spells/G2Spells.rc" }

	files { "G2Spells/g2spells.hpp" }
	files { "G2Spells/g2spells.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC26_DIR"))
	addpostbuildcopy(os.getenv("GOTHIC12_DIR"))
	setdebugpath(os.getenv("GOTHIC12_DIR"), "Gothic2.exe", "")

project "S1Fixes"
	kind "SharedLib"
	language "C++"
	targetname "s1fixes"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "G12Dll/G12.h" }
	files { "G12Dll/G12Core.h" }
	files { "G12Dll/S1.h" }
	files { "G12Dll/S1.hpp" }
	files { "G12Dll/MemoryMgr.h" }
	files { "G12Dll/version.h" }

	files { "S1Fixes/resource.h" }
	files { "S1Fixes/S1Fixes.rc" }

	files { "S1Fixes/s1fixes.hpp" }
	files { "S1Fixes/s1fixes.cpp" }

	addpostbuildcopy(os.getenv("GOTHIC108K_DIR"))
	setdebugpath(os.getenv("GOTHIC108K_DIR"), "Spacer.exe", "")
