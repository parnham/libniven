solution "examples"
	language		"C++"
	targetdir		"lib"
	configurations	"default"
	platforms		"native"
	includedirs		"../../include"
	libdirs			"../../lib"
	toolset			"clang"
	buildoptions	{ "-Wall", "-Wno-sign-compare", "-std=c++17", "-O3", "-fPIC", "-D_FORTIFY_SOURCE=2" }
	excludes		{ "**.bak", "**~" }

	configuration "linux"
		symbols	"On"


	project "example"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "niven", "hiredis", "pthread", "stdc++fs"  }
		files		{ "example.cpp", "modules/**.cpp" }

	project "ssl"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "niven", "pthread"  }
		files		{ "ssl.cpp", "modules/Simple.cpp" }
