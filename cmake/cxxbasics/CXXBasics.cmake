cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Variables necessary in every module
include(InitCXXBasics)

# Set reasonable CMake defaults
include(DefaultSettings)

# Activate faster linkers by default
include(accelerators/UseFasterLinkers)

# Activate the compiler cache tool
include(accelerators/UseCompilerCacheTool)

# Allow the user to extend CXXBasics
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../cxxbasics-extension.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/../cxxbasics-extension.cmake")
endif()
