cmake_minimum_required(VERSION 3.22)

project(
niven-examples
VERSION 0.0.22
DESCRIPTION "example niven modules"
LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_LINKER clang++)


include_directories(../../include)
link_directories(../../build)
add_compile_options(-Wall -Wextra -Wpedantic -fno-omit-frame-pointer)

file(GLOB_RECURSE example_sources
	modules/*.cpp
	example.cpp
)

add_executable(example ${example_sources})
target_link_libraries(example PRIVATE niven hiredis pthread)

add_executable(example-ssl ssl.cpp modules/Simple.cpp)
target_link_libraries(example-ssl PRIVATE niven hiredis pthread)
