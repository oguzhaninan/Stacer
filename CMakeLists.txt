cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Stacer)

# Add conditionals based on branch & whatnot
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ByThaBranch.cmake")

# Adding features(build cache + faster linkers) and reasonable defaults(Debug build by default)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxxbasics/CXXBasics.cmake")

#PCRECPP
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindPCRECPP.cmake")

#sdbusplus
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Sdbusplus.cmake")

# Setting a cleaner directory structure for the generated binaries
set(CMAKE_BINARY_DIR        "${CMAKE_BINARY_DIR}/output")
set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_BINARY_DIR}/")
set(LIBRARY_OUTPUT_PATH     "${CMAKE_BINARY_DIR}/lib")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_BRANCH "${BTB_CURRENT_BRANCH}")

# Activating MOC and searching for the Qt5 dependencies
set(CMAKE_AUTOMOC ON)
find_package(Qt5  COMPONENTS  Core Gui Widgets Charts Svg Concurrent REQUIRED)

#
####!!! sdbusplus is a c++17 library . Sweet 17!!!###
# Setting the minimum C++ standard and passing the Qt-specific define
set(STR_CPP17 "-std=c++17")
if(CMAKE_REQUIRED_FLAGS)
    set(CMAKE_REQUIRED_FLAGS ${STR_CPP17} ${CMAKE_REQUIRED_FLAGS})
else()
    set(CMAKE_REQUIRED_FLAGS ${STR_CPP17})
endif()
message(STATUS "cmake_required_flags equals thus far: ${CMAKE_REQUIRED_FLAGS}")

set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_EXTENSIONS         YES)
set(CMAKE_CXX_STANDARD_REQUIRED  YES)
#set(CMAKE_CXX_STANDARD           11)
#set(CMAKE_CXX_EXTENSIONS         YES)
#set(CMAKE_CXX_STANDARD_REQUIRED  YES)
add_definitions(-DQT_DEPRECATED_WARNINGS) # omfg so annoying
# ## screw those messages >:P
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

# Configurations for system-specifics #
####
# sizeof's
include(CheckTypeSize)
check_type_size(bool SIZEOF_BOOL LANGUAGE CXX)
# constants
exec_program(/bin/cat
	ARGS "/proc/sys/kernel/threads-max"
	OUTPUT_VARIABLE MAXIMUM_THREADS)
message(STATUS "maximum allowed threads: ${MAXIMUM_THREADS}")

string(CONCAT TARGET_ARCHY "TARGET_" "${CXXBASICS_CXX_COMPILER_TARGET_ARCH}")
string(TOUPPER "${TARGET_ARCHY}" TARGET_ARCHY)

####
configure_file(Stacer-cfg.h.in Stacer-cfg.h)
file(COPY ${PROJECT_ROOT}/Stacer-cfg.h DESTINATION ${PROJECT_ROOT}/stacer/)
file(COPY ${PROJECT_ROOT}/Stacer-cfg.h DESTINATION ${PROJECT_ROOT}/stacer-core/)

# Subprojects
add_subdirectory(stacer-core)
add_subdirectory(stacer)

# Tests & Tests!!!
enable_testing()
add_subdirectory(tests)

