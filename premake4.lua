solution "psinc"
	language		"C++"
	targetdir		"lib"
	configurations	"default"
	platforms		"native"
	includedirs		"include"
	libdirs			"lib"
	buildoptions	{ "-Wall", "-Wno-sign-compare", "-std=c++11", "-O3", "-fPIC", "-D_FORTIFY_SOURCE=2" }
	excludes		{ "**.bak", "**~" }

	configuration "linux"
		flags	"Symbols"

	project "libniven"
		kind				"SharedLib"
		targetname			"niven"
		links				{ "emergent", "microhttpd" }
		linkoptions 		{ "-Wl,-soname,libniven.so.0" }
		files				{ "include/niven/**h", "src/niven/**.cpp" }
		configuration "linux"
			postbuildcommands	"./strip lib/libniven.so"

	project "example"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "emergent", "libniven", "entity" }
		files		{ "src/example/**.cpp" }

