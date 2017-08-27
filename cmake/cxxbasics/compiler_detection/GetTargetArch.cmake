# This module identifies the target architecture of the C and CXX compilers

# ARM("armv8" includes AArch64, "arm" is all the old ARM processors + Cortex-M): armv8, armv7, armv6, armv5, arm
# Itanium:                                                                       ia64
# Traditional PC architectures:                                                  x86, x86_64
# MIPS(RISC):                                                                    mipsI, mipsII, mipsIII, mipsIV, mipsV, mips32, mips64, mips
# PowerPC:                                                                       ppc64, ppc
# IBM System z:                                                                  s390, s390x
# SPARC:                                                                         sparcv9, sparc64, sparc

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

opt_ifndef("C compiler target architecture"    STRING  ""  CXXBASICS_C_COMPILER_TARGET_ARCH)
opt_ifndef("CXX compiler target architecture"  STRING  ""  CXXBASICS_CXX_COMPILER_TARGET_ARCH)
if("${CXXBASICS_C_COMPILER_TARGET_ARCH}" STREQUAL ""
    OR "${CXXBASICS_CXX_COMPILER_TARGET_ARCH}" STREQUAL "")
  include(helpers/FnMktemp)

  mktemp()
  if("${mktemp_result}" STREQUAL "")
    opt_overwrite(CXXBASICS_TMP_FOLDER "${CMAKE_BINARY_DIR}")
    mktemp()
  endif()

  ## Based on Qt 5.9 processor detection: https://github.com/qt/qtbase/blob/dev/src/corelib/global/qprocessordetection.h
  file(WRITE "${mktemp_result}"
      "
      // ARM
      #if defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__aarch64__)
      #  if defined(__ARM64_ARCH_8__) \\
            || defined(__aarch64__) \\
            || defined(__CORE_CORTEXAV8__) // GHS-specific for INTEGRITY
      #    define Q_PROCESSOR_ARM 8
      #  elif defined(__ARM_ARCH_7__) \\
            || defined(__ARM_ARCH_7A__) \\
            || defined(__ARM_ARCH_7R__) \\
            || defined(__ARM_ARCH_7M__) \\
            || defined(__ARM_ARCH_7S__) \\
            || defined(_ARM_ARCH_7) \\
            || defined(__CORE_CORTEXA__) // GHS-specific for INTEGRITY
      #    define Q_PROCESSOR_ARM 7
      #  elif defined(__ARM_ARCH_6__) \\
            || defined(__ARM_ARCH_6J__) \\
            || defined(__ARM_ARCH_6T2__) \\
            || defined(__ARM_ARCH_6Z__) \\
            || defined(__ARM_ARCH_6K__) \\
            || defined(__ARM_ARCH_6ZK__) \\
            || defined(__ARM_ARCH_6M__)
      #    define Q_PROCESSOR_ARM 6
      #  elif defined(__ARM_ARCH_5TEJ__) \\
              || defined(__ARM_ARCH_5TE__)
      #    define Q_PROCESSOR_ARM 5
      #  else
      #    define Q_PROCESSOR_ARM 0
      #  endif
      #  if Q_PROCESSOR_ARM >= 8
      #    error CMAKE_TARGET_ARCH armv8
      #  endif
      #  if Q_PROCESSOR_ARM >= 7
      #    error CMAKE_TARGET_ARCH armv7
      #  endif
      #  if Q_PROCESSOR_ARM >= 6
      #    error CMAKE_TARGET_ARCH armv6
      #  endif
      #  if Q_PROCESSOR_ARM >= 5
      #    error CMAKE_TARGET_ARCH armv5
      #  endif
      #  error CMAKE_TARGET_ARCH arm // old ARM, Cortex-M...
      #elif defined(__i386) || defined(__i386__) || defined(_M_IX86) // x86
      #  error CMAKE_TARGET_ARCH x86
      #elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64) // x86_64
      #  error CMAKE_TARGET_ARCH x86_64
      #elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64) // Itanium
      #  error CMAKE_TARGET_ARCH ia64
      #elif defined(__mips) || defined(__mips__) || defined(_M_MRX000) // MIPS(RISC)
      #  if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
      #    error CMAKE_TARGET_ARCH mipsI
      #  endif
      #  if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
      #    error CMAKE_TARGET_ARCH mipsII
      #  endif
      #  if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
      #    error CMAKE_TARGET_ARCH mipsIII
      #  endif
      #  if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
      #    error CMAKE_TARGET_ARCH mipsIV
      #  endif
      #  if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
      #    error CMAKE_TARGET_ARCH mipsV
      #  endif
      #  if defined(_MIPS_ARCH_MIPS32) || defined(__mips32) || (defined(__mips) && __mips - 0 >= 32)
      #    error CMAKE_TARGET_ARCH mips32
      #  endif
      #  if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
      #    error CMAKE_TARGET_ARCH mips64
      #  endif
      #  error CMAKE_TARGET_ARCH mips // Unknown MIPS
      #elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \\
            || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \\
            || defined(_M_MPPC) || defined(_M_PPC)
      #  if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
      #    error CMAKE_TARGET_ARCH ppc64 // PowerPC 64
      #  endif
      #  error CMAKE_TARGET_ARCH ppc // PowerPC
      #elif defined(__s390__) // IBM System z(s390/s390x)
      #  if defined(__s390x__)
      #    error CMAKE_TARGET_ARCH s390x
      #  endif
      #  error CMAKE_TARGET_ARCH s390
      #elif defined(__sparc__) // SPARC
      #  if defined(__sparc_v9__)
      #    error CMAKE_TARGET_ARCH sparcv9
      #  endif
      #  if defined(__sparc64__)
      #    error CMAKE_TARGET_ARCH sparc64
      #  endif
      #  error CMAKE_TARGET_ARCH sparc
      #endif

      #error CMAKE_TARGET_ARCH unknown
      ")

  macro(__cxxbasics_define_arch  suffix  variable)
    file(RENAME "${mktemp_result}" "${mktemp_result}${suffix}")
    set(mktemp_result "${mktemp_result}${suffix}")

    try_compile(run_unused_result
        "${CMAKE_CURRENT_BINARY_DIR}"
        SOURCES "${mktemp_result}"
        OUTPUT_VARIABLE TARGET_ARCH
    )

    if("${TARGET_ARCH}" MATCHES "CMAKE_TARGET_ARCH")
      # Extracting the first "CMAKE_TARGET_ARCH [arch]"
      string(REGEX MATCH "CMAKE_TARGET_ARCH ([A-Za-z0-9_]+)" TARGET_ARCH "${TARGET_ARCH}")

      # Remove "CMAKE_TARGET_ARCH" and leaving only the architecture
      string(REPLACE "CMAKE_TARGET_ARCH " "" TARGET_ARCH "${TARGET_ARCH}")

      # Lets see if it is not unknown, otherwise we know we have the correct architecture in TARGET_ARCH
      if("${TARGET_ARCH}" STREQUAL "unknown")
        opt_overwrite(${variable} "unknown")
      else()
        opt_overwrite(${variable} "${TARGET_ARCH}")
      endif()
    else()
      # If for some reason we didn't get the expected string, set the arch to "unknown"
      opt_overwrite(${variable} "unknown")
    endif()

    # Catching coding errors
    if("${variable}" STREQUAL "")
      opt_overwrite(${variable} "unknown")
    endif()

    cbmessage("`${variable}` set to \"${${variable}}\"")
  endmacro()

  __cxxbasics_define_arch(".c"    CXXBASICS_C_COMPILER_TARGET_ARCH)
  __cxxbasics_define_arch(".cxx"  CXXBASICS_CXX_COMPILER_TARGET_ARCH)
endif()
