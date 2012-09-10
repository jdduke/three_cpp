cmake_minimum_required (VERSION 2.6)
project(glew)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  set(BUILD_64_BIT FALSE CACHE BOOL "Enable 64-bit build")
  if (BUILD_64_BIT)
    set(COMMON_FLAGS "-m64")
    set(GLEW_SIZE_TYPE x64)
  else()
    set(COMMON_FLAGS "-m32")
    set(GLEW_SIZE_TYPE x86)
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
    set(GLEW_SIZE_TYPE x86)
    set(BUILD_64_BIT FALSE)
  else()
    set(GLEW_SIZE_TYPE x64)
    set(BUILD_64_BIT TRUE)
  endif()
endif()

#############

set(GLEW_OUTPUT_SUBDIR ${GLEW_SIZE_TYPE}${GLEW_SUFFIX})
set(GLEW_BINARY_PATH  ${CMAKE_HOME_DIRECTORY}/bin/${GLEW_OUTPUT_SUBDIR})
set(GLEW_LIBRARY_PATH ${CMAKE_HOME_DIRECTORY}/lib/${GLEW_OUTPUT_SUBDIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${GLEW_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Executables.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${GLEW_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${GLEW_LIBRARY_PATH}
    CACHE PATH
    "Single Directory for all static libraries.")

#############

find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h
                           PATHS include)
mark_as_advanced(GLEW_INCLUDE_DIR)

include_directories(${GLEW_INCLUDE_DIR})

set(GLEW_SRC src/glew.c)
set(GLEW_HDR include/GL/glew.h)

add_definitions(-DGLEW_BUILD -DGLEW_NO_GLU)
add_library(glew  SHARED ${GLEW_SRC} ${GLEW_HDR})
add_library(glews STATIC ${GLEW_SRC} ${GLEW_HDR})

if (MSVC)
  set(GLEW_DEP_LIBS glu32 opengl32 gdi32 user32 kernel32)
elseif (MINGW)
  set(GLEW_DEP_LIBS glu32 opengl32 gdi32 user32 kernel32)
else ()
  set(GLEW_DEP_LIBS glu32 opengl32)
endif ()

target_link_libraries(glew  ${GLEW_DEP_LIBS})
target_link_libraries(glews ${GLEW_DEP_LIBS})

set_target_properties(glews PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC")

