# searches for https://github.com/openbmc/sdbusplus via pkg-config
# SDBUSPLUS_xxx vars
#

include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
	message(FATAL_ERROR "pkg-config is required but not found!")
endif()
mark_as_advanced(PKG_CONFIG_FOUND)

pkg_check_modules(SDBUSPLUS sdbusplus)

if(NOT SDBUSPLUS_FOUND)
	message(FATAL_ERROR "libsdbusplus is required library!\nclone: https://github.com/openbmc/sdbusplus")
endif()

mark_as_advanced(SDBUSPLUS_LIBRARIES)
mark_as_advanced(SDBUSPLUS_INCLUDE_DIRS)
mark_as_advanced(SDBUSPLUS_LIBRARY_DIRS)

