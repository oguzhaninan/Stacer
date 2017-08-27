# This module activates a compiler cache

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

opt_ifndef("Use a compiler cache tool, if supported"  BOOL  ON  CXXBASICS_ACTIVATE_COMPILER_CACHE)
if(CXXBASICS_ACTIVATE_COMPILER_CACHE)
  if(CMAKE_HOST_UNIX)
    include(accelerators/UseCCache)
  endif()

  if(NOT CMAKE_C_COMPILER_LAUNCHER OR NOT CMAKE_CXX_COMPILER_LAUNCHER)
    include(accelerators/UseSCCache)
  endif()

  if(CMAKE_C_COMPILER_LAUNCHER)
    cbok("Compiler cache tool \"${CMAKE_C_COMPILER_LAUNCHER}\" set for the C compiler")
  else()
    cbnok("Could not set a compiler cache tool for the C compiler")
  endif()

  if(CMAKE_CXX_COMPILER_LAUNCHER)
    cbok("Compiler cache tool \"${CMAKE_CXX_COMPILER_LAUNCHER}\" set for the CXX compiler")
  else()
    cbnok("Could not set a compiler cache tool for the CXX compiler")
  endif()
endif()
