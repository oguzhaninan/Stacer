# This module activates "ccache" support on Unix
# This module is supposed to be used only from "UseCompilerCacheTool.cmake"

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

find_program(__cxxbasics_ccache_found  ccache)
if(__cxxbasics_ccache_found)
  if(NOT CMAKE_C_COMPILER_LAUNCHER)
    set(CMAKE_C_COMPILER_LAUNCHER    ccache)
  endif()

  if(NOT CMAKE_CXX_COMPILER_LAUNCHER)
    set(CMAKE_CXX_COMPILER_LAUNCHER  ccache)
  endif()
endif()
