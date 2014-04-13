project(three_examples)

set(THREE_EXAMPLE_DEP_LIBS ${THREE_LIB} ${THREE_DEP_LIBS} ${SDLMAIN_LIBRARY} ${GLEW_LIBRARY})

if(APPLE)
  if (${CMAKE_GENERATOR} STREQUAL Xcode OR NOT SDLMAIN_LIBRARY)
    remove_cxx_flag("${THREE_CXX_FLAGS}")
    add_library(apple_sdlmain_library STATIC ../externals/sdl-1.2.15/xcode/SDLMain.m)
    set(THREE_EXAMPLE_DEP_LIBS ${THREE_EXAMPLE_DEP_LIBS} apple_sdlmain_library)
  endif()
  find_library(COCOA_LIBRARY Cocoa)
  set(THREE_EXAMPLE_DEP_LIBS ${THREE_EXAMPLE_DEP_LIBS} ${COCOA_LIBRARY})
endif()

file(GLOB_RECURSE THREE_EXAMPLES_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/extras/*.cpp)
file(GLOB_RECURSE THREE_EXAMPLES_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/extras/*.h)
set(THREE_EXAMPLES_LIB_SOURCE_FILES ${THREE_EXAMPLES_LIB_SOURCES} ${THREE_EXAMPLES_LIB_HEADERS})
three_add_static_library(three_examples ${THREE_EXAMPLES_LIB_SOURCE_FILES})
set_target_properties(three_examples PROPERTIES COMPILE_FLAGS "${THREE_CXX_FLAGS}")
target_link_libraries(three_examples ${THREE_EXAMPLE_DEP_LIBS})

set(THREE_EXAMPLE_LIBS ${THREE_EXAMPLE_DEP_LIBS} three_examples)

file(GLOB examples "*.cpp")

foreach(example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  three_add_example(${example_name} ${example})
  set_target_properties(${example_name} PROPERTIES COMPILE_FLAGS "${THREE_CXX_FLAGS}")
endforeach()

