project(three_examples)

set(THREE_EXAMPLE_LIBS ${THREE_LIB} ${THREE_DEP_LIBS} ${SDLMAIN_LIBRARY})

if(APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  set(THREE_EXAMPLE_LIBS ${THREE_EXAMPLE_LIBS} ${COCOA_LIBRARY})
endif()

file(GLOB examples "*.cpp")

foreach(example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  three_add_example(${example_name} ${example})
endforeach()

