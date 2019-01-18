# searches for https://github.com/openbmc/sdbusplus via pkg-config
# SDBUSPLUS_xxx vars
#
macro(build_sdbusplus)
	execute_process(COMMAND ./build-aux/build-sdpp.sh
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(build_sdbusplus)

if(DEFINED WILLBUILD_SDPP)
	return()
endif()

include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
	message(FATAL_ERROR "pkg-config is required but not found!")
endif()
mark_as_advanced(PKG_CONFIG_FOUND)

pkg_check_modules(SDBUSPLUS sdbusplus)

if(NOT SDBUSPLUS_FOUND)
	message(FATAL_ERROR "libsdbusplus is required library!\nclone: https://github.com/openbmc/sdbusplus")
else()
	link_directories(${SDBUSPLUS_LIBRARY_DIRS})
endif()

mark_as_advanced(SDBUSPLUS_LIBRARIES)
mark_as_advanced(SDBUSPLUS_INCLUDE_DIRS)
mark_as_advanced(SDBUSPLUS_LIBRARY_DIRS)

