solution "examples"
	language		"C++"
	targetdir		"lib"
	configurations	"default"
	platforms		"native"
	includedirs		"../../include"
	libdirs			"../../lib"
	toolset			"clang"
	buildoptions	{ "-Wall", "-Wno-sign-compare", "-std=c++14", "-O3", "-fPIC", "-D_FORTIFY_SOURCE=2" }
	excludes		{ "**.bak", "**~" }

	configuration "linux"
		flags	"Symbols"


	project "example"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "niven", "hiredis", "pthread"  }
		files		{ "example.cpp" }

	project "ssl"
		kind		"ConsoleApp"
		targetdir	"bin"
		links		{ "niven", "pthread"  }
		files		{ "ssl.cpp" }
