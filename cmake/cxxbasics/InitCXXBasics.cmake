## This module defines common functions and variables that should be accessible in every module

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Enable C and CXX by default. This allows to run some commands in script mode, or using "-C"
enable_language(C)
enable_language(CXX)

# Project custom messaging macros
include(helpers/MacroCustomMessages)

# Widely-used macros to handle the cache variables
include(helpers/MacroOpt)
