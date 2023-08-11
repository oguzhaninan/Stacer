cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Stacer)

# Adding features(build cache + faster linkers) and reasonable defaults(Debug build by default)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxxbasics/CXXBasics.cmake")

# Setting a cleaner directory structure for the generated binaries
set(CMAKE_BINARY_DIR        "${CMAKE_BINARY_DIR}/output")
set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_BINARY_DIR}/")
set(LIBRARY_OUTPUT_PATH     "${CMAKE_BINARY_DIR}/lib")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Activating MOC and searching for the Qt5 dependencies
set(CMAKE_AUTOMOC ON)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Gui Widgets Charts Svg Concurrent REQUIRED)

# Setting the minimum C++ standard and passing the Qt-specific define
set(CMAKE_CXX_STANDARD           11)
set(CMAKE_CXX_EXTENSIONS         YES)
set(CMAKE_CXX_STANDARD_REQUIRED  YES)
set(CMAKE_DEBUG_POSTFIX          d)
add_definitions(-DQT_DEPRECATED_WARNINGS)

# Subprojects
add_subdirectory(stacer-core)
add_subdirectory(stacer)
