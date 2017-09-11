## This module contains project-wide custom CMake messagging macros.
## Does not adhere to the overall style because MacroCbmessage does not sound very well nor represent all macros...

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# This does not work in Windows CMD(usually also CI)
if(CYGWIN OR NOT CMAKE_HOST_WIN32)
  string(ASCII 27 __cxxbasics_escape)
  set(__cxxbasics_prefix_color  "${__cxxbasics_escape}[36m") # Cyan
  set(__cxxbasics_success_color "${__cxxbasics_escape}[32m") # Green
  set(__cxxbasics_failure_color "${__cxxbasics_escape}[31m") # Red
  set(__cxxbasics_no_color      "${__cxxbasics_escape}[m")   # Reset color

  unset(__cxxbasics_escape)
endif()

set(__cxxbasics_prefix  "[${__cxxbasics_prefix_color}cxxbasics${__cxxbasics_no_color}]")
set(__cxxbasics_success "[${__cxxbasics_success_color}✓${__cxxbasics_no_color}]")
set(__cxxbasics_failure "[${__cxxbasics_failure_color}✗${__cxxbasics_no_color}]")

unset(__cxxbasics_prefix_color)
unset(__cxxbasics_success_color)
unset(__cxxbasics_failure_color)
unset(__cxxbasics_no_color)

#========================================================

# Use `_cbp` when displaying a simple message.
# `cbp` stands for [C]XX[B]asics [P]refix
set(_cbp   "${__cxxbasics_prefix}")

# Use `_cbok` when displaying a notification of success(ex: `cxxbasics` succeded to set up ccache)
# `cbok` stands for [C]XX[B]asics [OK]
set(_cbok  "${_cbp}${__cxxbasics_success}")

# Use `_cbnok` when displaying a notification of failure(ex: the user activated `ccache` but it was not found in the system)
# `cbnok` stands for [C]XX[B]asics [N]ot [OK]
set(_cbnok "${_cbp}${__cxxbasics_failure}")

macro(cbmessage)
  message(STATUS "${_cbp} " ${ARGV})
endmacro(cbmessage)

macro(cbok)
  message(STATUS "${_cbok} " ${ARGV})
endmacro(cbok)

macro(cbnok)
  message(STATUS "${_cbnok} " ${ARGV})
endmacro(cbnok)

macro(cberror)
  message(FATAL_ERROR "${_cbnok} " ${ARGV})
endmacro(cberror)

#========================================================

unset(__cxxbasics_prefix)
unset(__cxxbasics_success)
unset(__cxxbasics_failure)
