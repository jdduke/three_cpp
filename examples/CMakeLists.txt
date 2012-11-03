project(three_examples)

set(THREE_EXAMPLE_LIBS ${THREE_LIB} ${THREE_DEP_LIBS})

file(GLOB examples "*.cpp")
#set(examples lines_cubes.cpp)

foreach(example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  three_add_example(${example_name} ${example})
endforeach()

