project(three_examples)

set(HEADER_ONLY TRUE CACHE BOOL
  "Whether to use Three.cpp as a header-only library")

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
  set(GLEW_LIBRARY glews)
endif()

include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SDL_INCLUDE_DIR})
include_directories(${RAPID_JSON_INCLUDE_DIR})

add_definitions(-DGLEW_STATIC -DTHREE_SDL -DTHREE_GLEW)

set(EXAMPLE_LIBS ${GLEW_LIBRARY}
                ${SDL_LIBRARY}
                ${SDLIMAGE_LIBRARY}
                ${SDLTTF_LIBRARY}
                ${OPENGL_LIBRARIES})

if (NOT HEADER_ONLY)
  add_definitions(-DTHREE_SEPARATE_COMPILATION)
  add_library(three_impl STATIC three.cpp three_extras.cpp)
  target_link_libraries(three_impl ${EXAMPLE_LIBS})
  set(EXAMPLE_LIBS ${EXAMPLE_LIBS} three_impl)
endif()

set(examples simple
             trails
             geometry_hierarchy
             geometry_hierarchy_2
             test_memory
             lines_colors
             lines_cubes
             shader)

foreach(example ${examples})
  add_executable(${example} ${example}.cpp)
  target_link_libraries(${example} ${EXAMPLE_LIBS})
endforeach()
