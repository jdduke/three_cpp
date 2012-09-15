project(three_examples)

set(SDL_ROOT "" CACHE PATH
	"The location of the SDL install prefix (only used if the SDL is not yet found)")
if(SDL_ROOT)
  set(CMAKE_PREFIX_PATH ${SDL_ROOT})
elseif(MSVC)
  set(CMAKE_PREFIX_PATH ${EXTERNALS_DIR}/sdl-1.2.15/msvc)
elseif(MINGW)
  set(CMAKE_PREFIX_PATH ${EXTERNALS_DIR}/sdl-1.2.15/mingw)
endif()

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
if (NOT WIN32)
  find_package(GLEW REQUIRED)
else()
  set(GLEW_LIBRARY glews)
endif()

include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SDL_INCLUDE_DIR})

add_definitions(-DGLEW_STATIC -DTHREE_SDL -DTHREE_GLEW)

#add_executable(simple ${THREE_HEADERS} simple.cpp)
add_executable(simple simple.cpp)

target_link_libraries(simple ${GLEW_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARIES})

