cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Stacer)

# Build with PIC and PIE(default: ON)
option(CMAKE_POSITION_INDEPENDENT_CODE "Determines whether position independent executables or shared libraries will be created" ON)

# Build with LTO(default: OFF)
option(CMAKE_INTERPROCEDURAL_OPTIMIZATION "If set to true, enables interprocedural optimizations if they are known to be supported by the compiler" OFF)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Apply PIE flags if cmake_minimum_required is over 3.14(see policy CMP0083)
if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
    include(CheckPIESupported)
    check_pie_supported()
    set(CMAKE_POLICY_DEFAULT_CMP0083 NEW)
endif(POLICY CMP0083)

# Build with LTO, if explicitly enabled
if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    if(POLICY CMP0069)
        cmake_policy(SET CMP0069 NEW)
        include(CheckIPOSupported)
        check_ipo_supported()

        set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
        set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CXX_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_CXX_FLAGS}")
    endif(POLICY CMP0069)
endif(CMAKE_INTERPROCEDURAL_OPTIMIZATION)

# Setting a cleaner directory structure for the generated binaries
set(CMAKE_BINARY_DIR        "${CMAKE_BINARY_DIR}/output")
set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_BINARY_DIR}/")
set(LIBRARY_OUTPUT_PATH     "${CMAKE_BINARY_DIR}/lib")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Activating MOC and searching for the Qt5 dependencies
set(CMAKE_AUTOMOC ON)
find_package(Qt5  COMPONENTS  Core Gui Widgets Charts Svg Concurrent REQUIRED)

# Setting the minimum C++ standard and passing the Qt-specific define
set(CMAKE_CXX_STANDARD           11)
set(CMAKE_CXX_EXTENSIONS         YES)
set(CMAKE_CXX_STANDARD_REQUIRED  YES)
add_definitions(-DQT_DEPRECATED_WARNINGS)

# Subprojects
add_subdirectory(stacer-core)
add_subdirectory(stacer)
