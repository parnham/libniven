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
