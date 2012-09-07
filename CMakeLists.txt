cmake_minimum_required (VERSION 2.6)
project(three)

set(CMAKE_DEBUG_POSTFIX "d")

# Treat warnings as errors cmake option
set(TREAT_WARNINGS_AS_ERRORS  TRUE  CACHE BOOL "Treat warnings as errors")
set(BUILD_THREE_EXAMPLES TRUE  CACHE BOOL "Build three.cpp examples")
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

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  set(BUILD_64_BIT FALSE CACHE BOOL "Enable 64-bit build")
  if (BUILD_64_BIT)
    set(COMMON_FLAGS "-m64")
    set(THREE_SIZE_TYPE x64)
  else()
    set(COMMON_FLAGS "-m32")
    set(THREE_SIZE_TYPE x86)
  endif()

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
  endif()

  if (MINGW)
    set(COMMON_FLAGS "${COMMON_FLAGS} -static-libstdc++ -static-libgcc -static")
  endif()

  if (CMAKE_COMPILER_IS_CLANGXX)
    set(COMMON_FLAGS "${COMMON_FLAGS} -stdlib=libc++")
    add_definitions("-std=c++0x")
    add_definitions("-DGTEST_USE_OWN_TR1_TUPLE=1")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions("-D_GLIBCXX_USE_NANOSLEEP")
  endif()

  set(CMAKE_C_FLAGS             "${CMAKE_C_FLAGS}             ${COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}           ${COMMON_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    ${COMMON_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COMMON_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${COMMON_FLAGS}")
endif()

#############

if(MSVC)
  if(CMAKE_SIZEOF_VOID_P MATCHES 4)
    set(THREE_SIZE_TYPE x86)
    set(BUILD_64_BIT FALSE)
  else()
    set(THREE_SIZE_TYPE x64)
    set(BUILD_64_BIT TRUE)
  endif()

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

include_directories(.)

file(GLOB THREE_HEADERS three/*.hpp)
source_group(three FILES ${THREE_HEADERS})

if(BUILD_THREE_EXAMPLES)
  add_subdirectory(examples)
endif()

#if(BUILD_THREE_TESTS)
#  include_directories(${THREE_SOURCE_DIR}/tests)
#  add_subdirectory(externals/googletest)
#  add_subdirectory(tests)
#endif()
