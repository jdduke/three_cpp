cmake_minimum_required (VERSION 2.6)
project(three)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/externals)

include(${PROJECT_SOURCE_DIR}/cmake/ThreeTargets.cmake)

set(THREE_VERSION 0.0.1 CACHE STRING "three.cpp version")
dissect_version()

set(THREE_TREAT_WARNINGS_AS_ERRORS FALSE CACHE BOOL "Treat warnings as errors")
set(THREE_BUILD_EXAMPLES     TRUE CACHE BOOL "Build three.cpp examples")
set(THREE_BUILD_TESTS        FALSE CACHE BOOL "Build three.cpp unit tests")
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

set(THREE_CXX_FLAGS "" CACHE INTERNAL "" FORCE)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  if (NOT ANDROID)
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
    set(THREE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
  elseif (MINGW OR ANDROID)
    set(THREE_CXX_FLAGS "-std=c++11")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  else()
    set(THREE_CXX_FLAGS "-std=c++0x")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  endif()

  set(CMAKE_C_FLAGS             "${CMAKE_C_FLAGS}             ${THREE_COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}           ${THREE_COMMON_FLAGS} ${THREE_CXX_FLAGS}")
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

  if (MSVC_VERSION EQUAL 1700)
    add_definitions("-D_VARIADIC_MAX=10")
  endif()

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

if (NOT THREE_LIBRARY_STATIC)
  add_definitions(-DTHREE_DYN_LINK)
endif()

if (ANDROID)
  add_definitions(-DTHREE_GLES)
endif()

# TODO Refactor, add groupings, etc
file(GLOB_RECURSE THREE_SOURCES three/*.cpp)
file(GLOB_RECURSE THREE_HEADERS three/*.h)

set(THREE_LIB_SOURCE_FILES ${THREE_SOURCES} ${THREE_HEADERS})

set(THREE_LIB three)
three_add_library(${THREE_LIB} ${THREE_LIB_SOURCE_FILES})
set_target_properties(${THREE_LIB} PROPERTIES COMPILE_DEFINITIONS "THREE_SOURCE")

if(THREE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(THREE_BUILD_TESTS)
  add_subdirectory(${EXT_PROJECTS_DIR}/googletest)

  enable_testing()
  include_directories(${GTEST_INCLUDE_DIR} ${COMMON_INCLUDES})

  file(GLOB_RECURSE TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
  file(GLOB_RECURSE TEST_SRC_FILES ${TEST_SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

  add_executable(three_test ${TEST_SRC_FILES})
  add_dependencies(three_test googletest)
  add_dependencies(three_test three)
  if(MSVC)
    target_link_libraries(three_test
      debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
      optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
    target_link_libraries(three_test
      debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
      optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
  else()
    target_link_libraries(three_test
      ${GTEST_LIBS_DIR}/libgtest.a
      ${GTEST_LIBS_DIR}/libgtest_main.a
    )
  endif()
  target_link_libraries(three_test ${THREE_LIB})
  add_test(threetest ${THREE_BINARY_PATH}/three_tests -V)
endif()
