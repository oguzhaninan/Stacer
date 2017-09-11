## This module defines helper macros to set options(cached variables)

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# @macro opt
# Macro helper to set a cache value.
# Does not overwrite the value if it was already cached.
#
# `description` - the description that will be displayed in CMake cache editor
# `var_type`    - the type of the variable(BOOL, FILEPATH, PATH, STRING, INTERNAL)
# `var_value`   - the value `var_name` will be set to
# `var_name`    - variable name
macro(opt  description  var_type  var_value  var_name)
  set(${var_name}  ${var_value} CACHE ${var_type}  "${description}")

  # Stores internally information about this variable's description and type
  # Will be reused in `opt_overwrite` to make the macro easy to use
  set(${var_name}_DESCRIPTION "${description}" CACHE INTERNAL "")
  set(${var_name}_TYPE        "${var_type}"    CACHE INTERNAL "")
endmacro()

# @macro opt_ifndef
# Macro helper to set a cache value.
# Sets the cache value only if the variable(including local variables) was not defined or it is set to an empty string.
macro(opt_ifndef  description  var_type  var_value  var_name)
  if(NOT DEFINED ${var_name} OR "${${var_name}}" STREQUAL "")
    set(${var_name}  ${var_value} CACHE ${var_type}  "${description}"  FORCE)

    set(${var_name}_DESCRIPTION "${description}" CACHE INTERNAL "")
    set(${var_name}_TYPE        "${var_type}"    CACHE INTERNAL "")
  endif()
endmacro()

# @macro opt_force
# Macro helper to set a cache value.
# Sets the cache value or overwrites the value if the variable already exists.
macro(opt_force  description  var_type  var_value  var_name)
  set(${var_name}  ${var_value} CACHE ${var_type}  "${description}"  FORCE)

  set(${var_name}_DESCRIPTION "${description}" CACHE INTERNAL "")
  set(${var_name}_TYPE        "${var_type}"    CACHE INTERNAL "")
endmacro()

# @macro opt_overwrite
# Macro helper to set a cache value.
# Overwrites the cache value only if the variable already exists in the cache(not local variables).
# The variable in the cache has to be registered with one of the `opt` macros
macro(opt_overwrite  var_name  var_value)
  # we do not check `if(NOT DEFINED ${var_name})` because local variables don't limit us from updating the
  # correct variable in the cache. We rely on _DESCRIPTION and _TYPE to find if the variable was
  # previously registered with `opt`
  if(NOT DEFINED ${var_name}_DESCRIPTION OR NOT DEFINED ${var_name}_TYPE)
    cberror("user-code logic error: `${var_name}` was not registered with an `opt` macro beforehand")
  endif()

  set(${var_name}  ${var_value} CACHE ${${var_name}_TYPE}  "${${var_name}_DESCRIPTION}"  FORCE)
endmacro()
