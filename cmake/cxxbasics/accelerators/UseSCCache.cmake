# This module activates "sccache" support on Unix and Windows if
# another compiler cache tool was not found.
# This module is supposed to be used only from "UseCompilerCacheTool.cmake"

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

find_program(__cxxbasics_sccache_found  sccache)
if(__cxxbasics_sccache_found)
  if(NOT CMAKE_C_COMPILER_LAUNCHER)
    set(CMAKE_C_COMPILER_LAUNCHER    sccache)
  endif()

  if(NOT CMAKE_CXX_COMPILER_LAUNCHER)
    set(CMAKE_CXX_COMPILER_LAUNCHER  sccache)
  endif()
endif()
