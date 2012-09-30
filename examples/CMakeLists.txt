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
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
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

set(SAMPLE_LIBS ${GLEW_LIBRARY}
                ${SDL_LIBRARY}
                ${SDLIMAGE_LIBRARY}
                ${SDLTTF_LIBRARY}
                ${OPENGL_LIBRARIES})

if (NOT HEADER_ONLY)
  add_definitions(-DTHREE_SEPARATE_COMPILATION)
  add_library(three_impl STATIC three.cpp three_extras.cpp)
  target_link_libraries(three_impl ${SAMPLE_LIBS})
  set(SAMPLE_LIBS ${SAMPLE_LIBS} three_impl)
endif()

add_executable(simple simple.cpp)
add_executable(trails trails.cpp)
add_executable(geometry_hierarchy geometry_hierarchy.cpp)
add_executable(geometry_hierarchy_2 geometry_hierarchy_2.cpp)
add_executable(test_memory test_memory.cpp)

target_link_libraries(simple ${SAMPLE_LIBS})
target_link_libraries(trails ${SAMPLE_LIBS})
target_link_libraries(geometry_hierarchy ${SAMPLE_LIBS})
target_link_libraries(geometry_hierarchy_2 ${SAMPLE_LIBS})
target_link_libraries(test_memory ${SAMPLE_LIBS})
