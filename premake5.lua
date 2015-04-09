solution "niven"
	language		"C++"
	targetdir		"lib"
	configurations	"default"
	platforms		"native"
	includedirs		"include"
	libdirs			"lib"
	toolset			"clang"
	buildoptions	{ "-Wall", "-Wno-sign-compare", "-std=c++14", "-O3", "-fPIC", "-D_FORTIFY_SOURCE=2" }
	excludes		{ "**.bak", "**~" }

	configuration "linux"
		flags	"Symbols"

	project "libniven"
		kind				"SharedLib"
		targetname			"niven"
		-- defines				"NIVEN_USE_H2O"
		-- links				{ "h2o", "uv", "ssl", "crypto" }
		links				{ "emergent", "microhttpd", "gcrypt" }
		linkoptions 		{ "-Wl,-soname,libniven.so.0" }
		files				{ "include/niven/**h", "src/niven/**.cpp" }
		configuration "linux"
			postbuildcommands	"./strip lib/libniven.so"

	project "simple"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "emergent", "libniven", "hiredis", "pthread"  }
		files		{ "src/examples/simple.cpp" }

	project "test"
		kind				"ConsoleApp"
		targetdir			"bin"
		targetname			"test"
		files				{ "src/test/**.cpp" }
		postbuildcommands	{ "./bin/test" }
