cmake_minimum_required (VERSION 2.6)
project(glew)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)

  if(MINGW)
    set(GLEW_BUILD_64_BIT FALSE CACHE BOOL "Enable 64-bit build")
  else()
    set(GLEW_BUILD_64_BIT TRUE CACHE BOOL "Enable 64-bit build")
  endif()

  if (GLEW_BUILD_64_BIT)
    set(COMMON_FLAGS "-m64" CACHE INTERNAL "Common flags" FORCE)
    set(GLEW_SIZE_TYPE x64 CACHE INTERNAL "" FORCE)
  else()
    set(COMMON_FLAGS "-m32" CACHE INTERNAL "Common flags" FORCE)
    set(GLEW_SIZE_TYPE x86 CACHE INTERNAL "" FORCE)
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
    set(GLEW_BUILD_64_BIT FALSE)
  else()
    set(GLEW_SIZE_TYPE x64)
    set(GLEW_BUILD_64_BIT TRUE)
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

