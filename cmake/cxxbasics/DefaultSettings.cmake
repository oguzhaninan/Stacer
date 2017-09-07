# This module sets reasonable defaults that probably every C/C++ CMake project should

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Default build type "Debug"
opt_ifndef("Build Type(Debug, Release, RelWithDebInfo, MinSizeRel)"  STRING  "Debug"  CMAKE_BUILD_TYPE)

# Generate "compile_commands.json" - tools like clang-tidy can be run on this file
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)
