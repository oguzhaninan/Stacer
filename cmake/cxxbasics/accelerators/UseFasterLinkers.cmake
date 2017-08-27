# This module activates faster linkers, if these are available and supported.
# It prefers the fastest linker available(as of this writing LLD -> GNU gold -> ...)
# The linker is handled separately per compiler, so, you can do something like this:
# -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=clang++

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

opt_ifndef("Use faster linkers(LLD, GNU gold...) if supported"  BOOL  ON  CXXBASICS_USE_FASTER_LINKERS)
if(CXXBASICS_USE_FASTER_LINKERS)
  macro(__cxxbasics_set_linker  compiler)
    # Lets check if the default linker is not actually LLD or GNU gold(ex: a symbolic link)
    execute_process(COMMAND ${compiler} -Wl,--version
        OUTPUT_VARIABLE __cxxbasics_ld_version
        ERROR_QUIET)

    if("${__cxxbasics_ld_version}" MATCHES "LLD")
      set(__cxxbasics_using_lld_linker ON)
    elseif("${__cxxbasics_ld_version}" MATCHES "GNU gold")
      set(__cxxbasics_using_gold_linker ON)
    else()
      set(__cxxbasics_using_default_linker ON)
    endif()

    # We don't do anything if the system linker already is the LLD linker, or links to it
    if(NOT __cxxbasics_using_lld_linker)
      # We try to set LLD first because it's the fastest linker currently
      if(NOT __cxxbasics_using_lld_linker)
        # LLD is currently production quality only on "x86_64"
        include(compiler_detection/GetTargetArch)
        if("${compiler}" STREQUAL "${CMAKE_C_COMPILER}")
          set(__cxxbasics_target_arch "${CXXBASICS_C_COMPILER_TARGET_ARCH}")
          set(__cxxbasics_current_compiler "CMAKE_C_COMPILER")
        elseif("${compiler}" STREQUAL "${CMAKE_CXX_COMPILER}")
          set(__cxxbasics_target_arch "${CXXBASICS_CXX_COMPILER_TARGET_ARCH}")
          set(__cxxbasics_current_compiler "CMAKE_CXX_COMPILER")
        else()
          cberror("Could not obtain CMAKE_C_COMPILER nor CMAKE_CXX_COMPILER")
        endif()

        if("${__cxxbasics_target_arch}" STREQUAL "x86_64")
          # Lets check if the compiler supports the LLD linker
          execute_process(COMMAND ${compiler} -fuse-ld=lld -Wl,--version
              OUTPUT_VARIABLE __cxxbasics_ld_version
              ERROR_QUIET)

          if("${__cxxbasics_ld_version}" MATCHES "LLD")
            if("${__cxxbasics_current_compiler}" STREQUAL "CMAKE_C_COMPILER")
              set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -fuse-ld=lld")
            else()
              set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fuse-ld=lld")
            endif()

            set(__cxxbasics_using_lld_linker ON)
            cbok("${__cxxbasics_current_compiler}(${compiler})'s linker set to: LLD linker")
          endif()
        endif("${__cxxbasics_target_arch}" STREQUAL "x86_64")
      endif()

      # We set the GNU gold linker if we failed to set LLD
      if(NOT __cxxbasics_using_lld_linker AND NOT __cxxbasics_using_gold_linker)
        execute_process(COMMAND ${compiler} -fuse-ld=gold -Wl,--version
            OUTPUT_VARIABLE __cxxbasics_ld_version
            ERROR_QUIET)

        if("${__cxxbasics_ld_version}" MATCHES "GNU gold")
          if("${__cxxbasics_current_compiler}" STREQUAL "CMAKE_C_COMPILER")
            set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -fuse-ld=gold")
          else()
            set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fuse-ld=gold")
          endif()

          set(__cxxbasics_using_gold_linker ON)
          cbok("${__cxxbasics_current_compiler}(${compiler})'s linker is set to: GNU gold linker")
        endif()
      endif()

      # If we failed to set LLD or the GNU gold linker, we fallback to the default linker
      if(NOT __cxxbasics_using_lld_linker AND NOT __cxxbasics_using_gold_linker)
        set(__cxxbasics_using_default_linker ON)
        cbnok("${__cxxbasics_current_compiler}(${compiler})'s linker set to the fallback: default linker")
      endif()
    endif(NOT __cxxbasics_using_lld_linker)

    unset(__cxxbasics_using_lld_linker)
    unset(__cxxbasics_using_gold_linker)
    unset(__cxxbasics_using_default_linker)
    unset(__cxxbasics_ld_version)
    unset(__cxxbasics_target_arch)
    unset(__cxxbasics_current_compiler)
  endmacro()

  # Set the linker for the C compiler
  __cxxbasics_set_linker("${CMAKE_C_COMPILER}")

  # Set the linker for the CXX compiler
  __cxxbasics_set_linker("${CMAKE_CXX_COMPILER}")
endif()
