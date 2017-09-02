## This module defines helper functions to create temporary files and folders in a system-agnostic way

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

opt_ifndef("CXXBasics temporary folder(uses system folder by default)"  PATH  ""  CXXBASICS_TMP_FOLDER)
macro(__cxxbasics_mktemp_helper)
  # Try to catch wrong usage
  if(NOT "${ARGV0}" STREQUAL "file" AND NOT "${ARGV0}" STREQUAL "directory")
    cberror("Wrong use of the macro")
  endif()

  # If `CXXBASICS_TMP_FOLDER` is not defined or set to an empty string, than we will try to set it to the system TMP
  if(NOT DEFINED CXXBASICS_TMP_FOLDER OR NOT IS_DIRECTORY "${CXXBASICS_TMP_FOLDER}")
    if(CMAKE_HOST_WIN32)
      opt_overwrite(CXXBASICS_TMP_FOLDER "$ENV{TMP}")
    elseif(CMAKE_HOST_UNIX)
      opt_overwrite(CXXBASICS_TMP_FOLDER "/tmp")
    else()
      cberror("Unsupported OS. Cannot set the temporary folder, please manually modify CXXBASICS_TMP_FOLDER in the cache")
    endif()
  endif(NOT DEFINED CXXBASICS_TMP_FOLDER OR NOT IS_DIRECTORY "${CXXBASICS_TMP_FOLDER}")

  # Lets make sure it's actually a directory
  if(NOT IS_DIRECTORY "${CXXBASICS_TMP_FOLDER}")
    cberror("`${CXXBASICS_TMP_FOLDER}` is not a folder. Please manually modify CXXBASICS_TMP_FOLDER in the cache")
  endif()

  # We will try to generate different random names until we are sure that it is unique for the path we try to use
  opt_ifndef("Project prefix to be used when creating files and folders"  STRING  "cxxbasics"  CXXBASICS_PROJECT_PREFIX)
  string(RANDOM LENGTH 16 random_generated_string)
  file(TO_NATIVE_PATH "${CXXBASICS_TMP_FOLDER}/${CXXBASICS_PROJECT_PREFIX}_${random_generated_string}" mktemp_result)
  while(EXISTS "${mktemp_result}")
    string(RANDOM LENGTH 16 random_generated_string)
    file(TO_NATIVE_PATH "${CXXBASICS_TMP_FOLDER}/${CXXBASICS_PROJECT_PREFIX}_${random_generated_string}" mktemp_result)
  endwhile(EXISTS "${mktemp_result}")

  # Here the behavior between `file` and `directory` splits, so we handle them separately
  if("${ARGV0}" STREQUAL "file")
    set(mktemp_result "${mktemp_result}" PARENT_SCOPE)
    file(WRITE "${mktemp_result}" "")

    # file(WRITE) should throw an error but we'll check anyway
    if(NOT EXISTS "${mktemp_result}" OR IS_DIRECTORY "${mktemp_result}")
      cbnok("Failed to create the temporary file")
      unset(mktemp_result PARENT_SCOPE)
    endif()
  else()
    set(mktemp_directory_result "${mktemp_result}")
    set(mktemp_directory_result "${mktemp_directory_result}" PARENT_SCOPE)
    file(MAKE_DIRECTORY "${mktemp_directory_result}")

    # file(MAKE_DIRECTORY) should throw an error but we'll check anyway
    if(NOT IS_DIRECTORY "${mktemp_directory_result}")
      cbnok("Failed to create the temporary folder")
      unset(mktemp_directory_result PARENT_SCOPE)
    endif()
  endif()
endmacro()

# @function mktemp
# @return mktemp_result - stores the path to the temporary file
function(mktemp)
  __cxxbasics_mktemp_helper("file")
endfunction()

# @function mktemp_directory
# @return mktemp_directory_result - stores the path to the temporary folder
function(mktemp_directory)
  __cxxbasics_mktemp_helper("directory")
endfunction()
