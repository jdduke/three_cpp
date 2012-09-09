project(three_examples)

include_directories(${GLEW_INCLUDE_PATH})

add_executable(simple ${THREE_HEADERS} simple.cpp)

target_link_libraries(simple glews)

