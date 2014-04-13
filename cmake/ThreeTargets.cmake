# Credit to PCL's cmake script

###############################################################################
# Pull the component parts out of the version number.
macro(DISSECT_VERSION)
  # Find version components
  string(REGEX REPLACE "^([0-9]+).*" "\\1"
    THREE_MAJOR_VERSION "${THREE_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1"
    THREE_MINOR_VERSION "${THREE_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1"
    THREE_REVISION_VERSION "${THREE_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1"
    THREE_CANDIDATE_VERSION "${THREE_VERSION}")
endmacro(DISSECT_VERSION)

# Only lnik the taget if necessary.
# _name The target name.
macro(LINK_IF_NEEDED _name)
  if(WIN32 AND MSVC)
    set_target_properties(${_name} PROPERTIES LINK_FLAGS_RELEASE /OPT:REF)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #set_target_properties(${_name} PROPERTIES LINK_FLAGS -Wl)
  elseif(__COMPILER_PATHSCALE)
    set_target_properties(${_name} PROPERTIES LINK_FLAGS -mp)
  else()
    set_target_properties(${_name} PROPERTIES LINK_FLAGS -Wl,--as-needed)
  endif()
endmacro()

###############################################################################
# Add a library target.
# _name The library name.
# ARGN The source files for the library.
macro(THREE_ADD_LIBRARY _name)

  if (THREE_LIBRARY_STATIC)
    add_library(${_name} STATIC ${ARGN})
  else()
    add_library(${_name} SHARED ${ARGN})
  endif()

  target_link_libraries(${_name} ${THREE_DEP_LIBS})

  if(THREE_DEPENDS)
    add_dependencies(${_name} ${THREE_DEPENDS})
  endif()

  link_if_needed(${_name})

  if(NOT THREE_LIBRARY_STATIC)
    set_target_properties(${_name} PROPERTIES COMPILE_DEFINITIONS "THREE_SOURCE")
  endif()

  if (THREE_PLATFORM_TOOLSET)
    set_target_properties(${_name}
      PROPERTIES
      PLATFORM_TOOLSET "${THREE_PLATFORM_TOOLSET}")
  endif()

endmacro(THREE_ADD_LIBRARY)

###############################################################################
# Add an example target.
# _name The example name.
# ARGN :
#    FILES the source files for the example
macro(THREE_ADD_EXAMPLE _name)

  add_executable(${_name} ${ARGN})
  target_link_libraries(${_name} ${THREE_EXAMPLE_LIBS})

  link_if_needed(${_name})

  if(WIN32 AND MSVC)
    set_target_properties(${_name} PROPERTIES DEBUG_OUTPUT_NAME ${_name}${CMAKE_DEBUG_POSTFIX}
                                              RELEASE_OUTPUT_NAME ${_name}${CMAKE_RELEASE_POSTFIX})
  endif(WIN32 AND MSVC)

  if(THREE_DEPENDS)
    add_dependencies(${_name} ${THREE_DEPENDS})
  endif()

  if (THREE_PLATFORM_TOOLSET)
    set_target_properties(${_name}
      PROPERTIES
      PLATFORM_TOOLSET "${THREE_PLATFORM_TOOLSET}")
  endif()

endmacro(THREE_ADD_EXAMPLE)

###############################################################################
# Add compile flags to a target (because CMake doesn't provide something so
# common itself).
# _name The target name.
# _flags The new compile flags to be added, as a string.
macro(THREE_ADD_CFLAGS _name _flags)
  get_target_property(_current_flags ${_name} COMPILE_FLAGS)
  if(NOT _current_flags)
      set_target_properties(${_name} PROPERTIES COMPILE_FLAGS ${_flags})
  else(NOT _current_flags)
      set_target_properties(${_name} PROPERTIES
          COMPILE_FLAGS "${_current_flags} ${_flags}")
  endif(NOT _current_flags)
endmacro(THREE_ADD_CFLAGS)


###############################################################################
# Add link flags to a target (because CMake doesn't provide something so
# common itself).
# _name The target name.
# _flags The new link flags to be added, as a string.
macro(THREE_ADD_LINKFLAGS _name _flags)
  get_target_property(_current_flags ${_name} LINK_FLAGS)
  if(NOT _current_flags)
      set_target_properties(${_name} PROPERTIES LINK_FLAGS ${_flags})
  else(NOT _current_flags)
      set_target_properties(${_name} PROPERTIES
          LINK_FLAGS "${_current_flags} ${_flags}")
  endif(NOT _current_flags)
endmacro(THREE_ADD_LINKFLAGS)

macro(remove_cxx_flag flag)
  string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endmacro()

macro(add_cxx_flag flag)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
endmacro()
