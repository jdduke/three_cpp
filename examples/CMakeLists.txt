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

include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SDL_INCLUDE_DIR})

add_definitions(-DGLEW_STATIC -DTHREE_SDL -DTHREE_GLEW)

add_executable(simple ${THREE_HEADERS} simple.cpp)

target_link_libraries(simple glews ${SDL_LIBRARY})

