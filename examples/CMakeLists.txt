project(three_examples)

add_definitions(-DGLEW_STATIC -DTHREE_SDL -DTHREE_GLEW)

set(EXAMPLE_LIBS ${THREE_LIB})# ${THREE_DEP_LIBS})

#add_custom_command(
#  TARGET three_impl
#  POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E make_directory ${THREE_BINARY_PATH}
#  COMMAND ${CMAKE_COMMAND} -E copy ${src} ${THREE_BINARY_PATH}/.
#  COMMENT "Copying ${src}")

file(GLOB examples "*.cpp")
#set(examples custom_attributes_particles.cpp)

foreach(example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  add_executable(${example_name} ${example})
  target_link_libraries(${example_name} ${EXAMPLE_LIBS})
endforeach()

