cmake_minimum_required (VERSION 2.6)
project(three)

set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Treat warnings as errors cmake option
set(TREAT_WARNINGS_AS_ERRORS TRUE CACHE BOOL "Treat warnings as errors")
set(BUILD_THREE_EXAMPLES     TRUE CACHE BOOL "Build three.cpp examples")
#set(BUILD_THREE_TESTS    FALSE CACHE BOOL "Build three.cpp unit tests")

#############

if(MSVC10)
  set(THREE_SUFFIX _vc10)
elseif(MSVC11)
  set(THREE_SUFFIX _vc11)
else()
  set(THREE_SUFFIX)
endif()

#############
# Clang/GCC

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  if (MINGW)
    set(BUILD_64_BIT FALSE CACHE BOOL "Enable 64-bit build")
  else()
    set(BUILD_64_BIT TRUE CACHE BOOL "Enable 64-bit build")
  endif()

  if (BUILD_64_BIT)
    set(COMMON_FLAGS "-m64" CACHE INTERNAL "Common flags" FORCE)
    set(THREE_SIZE_TYPE x64 CACHE INTERNAL "" FORCE)
  else()
    set(COMMON_FLAGS "-m32" CACHE INTERNAL "Common flags" FORCE)
    set(THREE_SIZE_TYPE x86 CACHE INTERNAL "" FORCE)
  endif()

  set(COMMON_FLAGS "${COMMON_FLAGS} -Wall -Wno-missing-braces")

  if(TREAT_WARNINGS_AS_ERRORS)
    set(COMMON_FLAGS "${COMMON_FLAGS} -Werror")
  endif()

  set(STOP_ON_FIRST_ERROR TRUE CACHE BOOL "Stop compilation on first error")
  if (STOP_ON_FIRST_ERROR)
    set(COMMON_FLAGS "${COMMON_FLAGS} -Wfatal-errors")
  endif()

  set(BUILD_DEBUG FALSE CACHE BOOL "Default to debug build")
  if (BUILD_DEBUG)
    set(COMMON_FLAGS "${COMMON_FLAGS} -g")
    add_definitions("-D_DEBUG")
  endif()

  if (MINGW)
    set(COMMON_FLAGS "${COMMON_FLAGS} -static-libstdc++ -static-libgcc -static")
  endif()

  if (CMAKE_COMPILER_IS_CLANGXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++0x")
    add_definitions("-DGTEST_USE_OWN_TR1_TUPLE=1")
  elseif (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
  endif()

  set(CMAKE_C_FLAGS             "${CMAKE_C_FLAGS}             ${COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}           ${COMMON_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    ${COMMON_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COMMON_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${COMMON_FLAGS}")
endif()

#############
# MSVC

if(MSVC)
  if(CMAKE_SIZEOF_VOID_P MATCHES 4)
    set(THREE_SIZE_TYPE x86)
    set(BUILD_64_BIT FALSE)
  else()
    set(THREE_SIZE_TYPE x64)
    set(BUILD_64_BIT TRUE)
  endif()

  add_definitions("-D_VARIADIC_MAX=6")
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")

  set(LINK_STATIC_RUNTIME OFF CACHE BOOL "Link statically against C++ runtime")
  if(LINK_STATIC_RUNTIME)
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

set(THREE_OUTPUT_SUBDIR ${THREE_SIZE_TYPE}${THREE_SUFFIX})
set(THREE_BINARY_PATH  ${CMAKE_HOME_DIRECTORY}/bin/${THREE_OUTPUT_SUBDIR})
set(THREE_LIBRARY_PATH ${CMAKE_HOME_DIRECTORY}/lib/${THREE_OUTPUT_SUBDIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${THREE_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Executables.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${THREE_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${THREE_LIBRARY_PATH}
    CACHE PATH
    "Single Directory for all static libraries.")

#############
# Library config

set(THREE_HEADER_ONLY TRUE CACHE BOOL "Whether to use three.cpp as a header-only library")

set(EXTERNALS_DIR ${CMAKE_HOME_DIRECTORY}/externals)
set(DATA_DIR      ${CMAKE_HOME_DIRECTORY}/data)

include_directories(.)

if (NOT THREE_HEADER_ONLY OR BUILD_THREE_EXAMPLES)

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
    set(GLEW_LIBRARY glew)
  endif()

  include_directories(${GLEW_INCLUDE_DIR})
  include_directories(${SDL_INCLUDE_DIR})
  include_directories(${RAPID_JSON_INCLUDE_DIR})

  set(THREE_DEP_LIBS ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL_LIBRARY})

  set(THREE_RELEASE_BUILD FALSE CACHE BOOL
  "Whether to compile examples for installation")

  if (THREE_RELEASE_BUILD)
    add_definitions(-DTHREE_DATA_DIR=".")
  else()
    add_definitions(-DTHREE_DATA_DIR="${DATA_DIR}")
  endif()

endif()

#############

if (NOT THREE_HEADER_ONLY)
  add_definitions(-DTHREE_SEPARATE_COMPILATION)
  add_library(three_impl STATIC three/impl/src.cpp three/impl/src_extras.cpp)
  target_link_libraries(three_impl ${THREE_DEP_LIBS})
  set(THREE_LIB three_impl)
endif()

#############

# Add a project containing all headers
if(MSVC)

  file(GLOB THREE_BASE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/three/*.hpp)
  source_group("three" FILES ${THREE_BASE_HEADERS})
  set(THREE_HEADERS ${THREE_BASE_HEADERS})

  set (THREE_SUBDIRS cameras core extras lights loaders materials objects renderers scenes textures utils)
  foreach(subdir ${THREE_SUBDIRS})
    file(GLOB THREE_SUBDIR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/three/${subdir}/*.hpp)
    source_group("three\\${subdir}" FILES ${THREE_SUBDIR_HEADERS})
    #source_group(${subdir} FILES ${THREE_SUBDIR_HEADERS})
    set(THREE_HEADERS ${THREE_HEADERS} ${THREE_SUBDIR_HEADERS})
    endforeach()

  add_library(three STATIC ${THREE_HEADERS})
  set_target_properties(three PROPERTIES LINKER_LANGUAGE CXX)

endif()

#############

if(BUILD_THREE_EXAMPLES)
  add_subdirectory(examples)
endif()

#if(BUILD_THREE_TESTS)
#  include_directories(${THREE_SOURCE_DIR}/tests)
#  add_subdirectory(externals/googletest)
#  add_subdirectory(tests)
#endif()
