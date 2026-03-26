cmake_minimum_required(VERSION 3.22)

project(
	libniven
	VERSION 0.0.22
	DESCRIPTION "a C++17 library that simplifies the task of creating a REST-based API"
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_LINKER clang++)


include_directories(include)
add_compile_options(-Wall -Wextra -Wpedantic -fno-omit-frame-pointer -fPIC -D_FORTIFY_SOURCE=2)
add_link_options(-rdynamic)

file(GLOB_RECURSE niven_sources src/niven/*.cpp)

add_library(niven SHARED ${niven_sources})
target_link_libraries(niven PRIVATE microhttpd gcrypt)
set_target_properties(niven PROPERTIES SOVERSION 0)

# file(GLOB_RECURSE test_sources src/test/*.cpp)
# add_executable(niven-tests ${test_sources})
# target_include_directories(niven-tests PUBLIC include)
# target_link_libraries(niven-tests PRIVATE freeimage)
# add_test(NAME entity-tests COMMAND niven-tests)


# solution "niven"
# 	language		"C++"
# 	targetdir		"lib"
# 	configurations	"default"
# 	platforms		"native"
# 	includedirs		"include"
# 	libdirs			"lib"
# 	toolset			"clang"
# 	buildoptions	{ "-Wall", "-Wextra", "-Wpedantic", "-std=c++17", "-O3", "-fPIC", "-D_FORTIFY_SOURCE=2" }
# 	excludes		{ "**.bak", "**~" }

# 	configuration "linux"
# 		symbols	"On"

# 	project "libniven"
# 		kind				"SharedLib"
# 		targetname			"niven"
# 		-- defines				"NIVEN_USE_H2O"
# 		-- links				{ "h2o", "uv", "ssl", "crypto" }
# 		links				{ "microhttpd", "gcrypt", "stdc++fs" }
# 		linkoptions 		{ "-Wl,-soname,libniven.so.0" }
# 		files				{ "include/niven/**h", "src/niven/**.cpp" }
# 		-- configuration "linux"
# 		-- 	postbuildcommands	"./strip lib/libniven.so"

# 	project "test"
# 		kind				"ConsoleApp"
# 		targetdir			"bin"
# 		targetname			"test"
# 		files				{ "src/test/**.cpp" }
# 		postbuildcommands	{ "./bin/test" }
