cmake_minimum_required (VERSION 2.6)
project(three)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(${PROJECT_SOURCE_DIR}/cmake/ThreeTargets.cmake)

set(THREE_VERSION 0.0.1 CACHE STRING "three.cpp version")
dissect_version()

set(THREE_TREAT_WARNINGS_AS_ERRORS FALSE CACHE BOOL "Treat warnings as errors")
#set(THREE_BUILD_EXAMPLES     TRUE CACHE BOOL "Build three.cpp examples")
#set(THREE_BUILD_TESTS        FALSE CACHE BOOL "Build three.cpp unit tests")
set(THREE_HEADER_ONLY        FALSE CACHE BOOL "Whether to use three.cpp as a header-only library")
set(THREE_LIBRARY_STATIC     TRUE CACHE BOOL "If building three.cpp as a library, build statically")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#############
# Clang/GCC Config

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  if (MINGW)
    set(THREE_BUILD_64_BIT FALSE CACHE BOOL "Enable 64-bit build")
  else()
    set(THREE_BUILD_64_BIT TRUE CACHE BOOL "Enable 64-bit build")
  endif()

  if (THREE_BUILD_64_BIT)
    set(THREE_COMMON_FLAGS "-m64" CACHE INTERNAL "Common flags" FORCE)
    set(THREE_SIZE_TYPE x64 CACHE INTERNAL "" FORCE)
  else()
    set(THREE_COMMON_FLAGS "-m32" CACHE INTERNAL "Common flags" FORCE)
    set(THREE_SIZE_TYPE x86 CACHE INTERNAL "" FORCE)
  endif()

  set(THREE_COMMON_FLAGS "${THREE_COMMON_FLAGS} -Wall -Wno-missing-braces -Wno-unused-private-field")

  if(THREE_TREAT_WARNINGS_AS_ERRORS)
    set(THREE_COMMON_FLAGS "${THREE_COMMON_FLAGS} -Werror")
  endif()

  set(THREE_STOP_ON_FIRST_ERROR TRUE CACHE BOOL "Stop compilation on first error")
  if (THREE_STOP_ON_FIRST_ERROR)
    set(THREE_COMMON_FLAGS "${THREE_COMMON_FLAGS} -Wfatal-errors")
  endif()

  if (MINGW)
    set(THREE_COMMON_FLAGS "${THREE_COMMON_FLAGS} -static-libstdc++ -static-libgcc -static")
  endif()

  if (CMAKE_COMPILER_IS_CLANGXX)
    message(status "std flags -stdlib=libc++ -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
  elseif (MINGW)
    message(status "std flags -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  else()
    message(status "std flags -std=c++0x")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  endif()

  set(CMAKE_C_FLAGS             "${CMAKE_C_FLAGS}             ${THREE_COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}           ${THREE_COMMON_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    ${THREE_COMMON_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${THREE_COMMON_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${THREE_COMMON_FLAGS}")
endif()

#############
# MSVC Config

if(MSVC)

  # TODO - Figure out how to set the Platform Toolset in MSVC project files
  #        ... without resorting to pre-build script madness
  #set(THREE_USE_NOV_2012_CTP TRUE CACHE BOOL "Enable usage of the Nov 2012 CTP for MSVC 2012")
  if (THREE_USE_NOV_2012_CTP)
    set(THREE_PLATFORM_TOOLSET "v120_CTP_Nov2012" CACHE STRING "Platform toolset" FORCE)
  else()
    set(THREE_PLATFORM_TOOLSET "" CACHE INTERNAL "Platform toolset" FORCE)
  endif()

  if(CMAKE_SIZEOF_VOID_P MATCHES 4)
    set(THREE_SIZE_TYPE x86 CACHE INTERNAL "" FORCE)
    set(THREE_BUILD_64_BIT FALSE CACHE INTERNAL "" FORCE)
  else()
    set(THREE_SIZE_TYPE x64 CACHE INTERNAL "" FORCE)
    set(THREE_BUILD_64_BIT TRUE CACHE INTERNAL "" FORCE)
  endif()

  add_definitions("-D_VARIADIC_MAX=6")
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")

  set(THREE_LINK_STATIC_RUNTIME OFF CACHE BOOL "Link statically against C++ runtime")
  if(THREE_LINK_STATIC_RUNTIME)
    foreach(flag_var CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "MSVC C Debug MT flags " FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "MSVC C Release MT flags " FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Debug MT flags " FORCE)
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Release MT flags " FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
  endif()
endif()

#############
# Output Dirs

if(MSVC10)
  set(THREE_DIR_SUFFIX _vc10)
elseif(MSVC11)
  set(THREE_DIR_SUFFIX _vc11)
else()
  set(THREE_DIR_SUFFIX "")
endif()

set(THREE_OUTPUT_SUBDIR ${THREE_SIZE_TYPE}${THREE_DIR_SUFFIX} CACHE INTERNAL "" FORCE)
set(THREE_BINARY_PATH  ${CMAKE_HOME_DIRECTORY}/bin/${THREE_OUTPUT_SUBDIR} CACHE INTERNAL "" FORCE)
set(THREE_LIBRARY_PATH ${CMAKE_HOME_DIRECTORY}/lib/${THREE_OUTPUT_SUBDIR} CACHE INTERNAL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${THREE_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Executables." FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${THREE_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Libraries" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${THREE_LIBRARY_PATH}
    CACHE PATH
    "Single Directory for all static libraries." FORCE)

#############
# Library config

set(EXTERNALS_DIR ${CMAKE_HOME_DIRECTORY}/externals)

include_directories(.)

if ((NOT THREE_HEADER_ONLY) OR THREE_BUILD_EXAMPLES)

  # Locate necessary headers/libraries

  set(CMAKE_PREFIX_PATH ${EXTERNALS_DIR}/rapidjson ${CMAKE_PREFIX_PATH})

  set(SDL_ROOT "" CACHE PATH
    "The location of the SDL install prefix (only used if the SDL is not yet found)")
  if(SDL_ROOT)
    set(CMAKE_PREFIX_PATH ${SDL_ROOT} ${CMAKE_PREFIX_PATH})
  elseif(MSVC)
    set(CMAKE_PREFIX_PATH ${EXTERNALS_DIR}/sdl-1.2.15/msvc ${CMAKE_PREFIX_PATH})
  elseif(MINGW)
    set(CMAKE_PREFIX_PATH ${EXTERNALS_DIR}/sdl-1.2.15/mingw ${CMAKE_PREFIX_PATH})
  endif()

  find_package(SDL REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(RapidJSON REQUIRED)

  if (NOT WIN32)
    find_package(GLEW REQUIRED)
  else()
    # Locating GLEW on Windows isn't worth it... just build it
    add_subdirectory(externals/glew-1.9.0)
    if (THREE_STATIC_GLEW)
      set(GLEW_LIBRARY glews CACHE INTERNAL "" FORCE)
    else()
      set(GLEW_LIBRARY glew CACHE INTERNAL "" FORCE)
    endif()
  endif()

  set(THREE_STATIC_GLEW OFF CACHE BOOL "Use static glew library")
  set(THREE_DEP_LIBS ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL_LIBRARY})
  include_directories(${GLEW_INCLUDE_DIR})
  include_directories(${SDL_INCLUDE_DIR})
  include_directories(${RAPID_JSON_INCLUDE_DIR})

  # TODO: Remove explicit sdl/glew dependencies
  add_definitions(-DTHREE_SDL -DTHREE_GLEW)
  if (THREE_STATIC_GLEW)
    add_definitions(-DGLEW_STATIC)
  endif()

  ## TODO: Remove hard-wired data directory path
  set(THREE_RELEASE_BUILD FALSE CACHE BOOL
    "Whether to compile examples for installation (changes data dir from absolute to relative reference")
  set(DATA_DIR ${CMAKE_HOME_DIRECTORY}/data)
  if (THREE_RELEASE_BUILD)
    add_definitions(-DTHREE_DATA_DIR=".")
  else()
    add_definitions(-DTHREE_DATA_DIR="${DATA_DIR}")
  endif()

endif()

#############

# Copy SDL runtime on Windows
if (MSVC OR MINGW)
  if ((NOT THREE_HEADER_ONLY) OR THREE_BUILD_EXAMPLES OR THREE_BUILD_TESTS)
    get_filename_component(SDL_PATH ${SDLMAIN_LIBRARY} PATH)
    set(SDL_RUNTIME_LIBRARY "${SDL_PATH}/SDL.dll" CACHE INTERNAL "" FORCE)
    message (STATUS "Copying ${SDL_RUNTIME_LIBRARY} to ${THREE_BINARY_PATH} \n")
    set(THREE_DEPENDS three_depends)
    if (MSVC)
      #add_custom_command(
      add_custom_target(
        ${THREE_DEPENDS}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${THREE_BINARY_PATH}/$<CONFIGURATION>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_RUNTIME_LIBRARY} "${THREE_BINARY_PATH}/$<CONFIGURATION>"
        COMMENT "Copying ${SDL_RUNTIME_LIBRARY}")
    else()
      add_custom_target(
        ${THREE_DEPENDS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${THREE_BINARY_PATH}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_RUNTIME_LIBRARY} "${THREE_BINARY_PATH}"
        COMMENT "Copying ${SDL_RUNTIME_LIBRARY}")
    endif()
  endif()
endif()

if (NOT THREE_HEADER_ONLY)
  add_definitions(-DTHREE_SEPARATE_COMPILATION)
  if (NOT THREE_LIBRARY_STATIC)
    add_definitions(-DTHREE_DYN_LINK)
  endif()

  # TODO Refactor, add groupings, etc
  file(GLOB_RECURSE THREE_SOURCES three/*.cpp)
  file(GLOB_RECURSE THREE_HEADERS three/*.h)

  set(THREE_LIB_SOURCE_FILES ${THREE_SOURCES} ${THREE_HEADERS})

  if(${CMAKE_GENERATOR} STREQUAL Xcode)
    set(THREE_LIB_SOURCE_FILES ${THREE_LIB_SOURCE_FILES} externals/sdl-1.2.15/xcode/SDLMain.m)
  endif()

  set(THREE_LIB three)
  three_add_library(${THREE_LIB} ${THREE_LIB_SOURCE_FILES})

endif()

if(THREE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

#if(THREE_BUILD_TESTS)
#  include_directories(${THREE_SOURCE_DIR}/tests)
#  add_subdirectory(externals/googletest)
#  add_subdirectory(tests)
#endif()
