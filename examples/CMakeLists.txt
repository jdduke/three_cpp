project(three_examples)

include_directories(${GLEW_INCLUDE_PATH})

add_executable(simple ${THREE_HEADERS} simple.cpp)

add_definitions(-DGLEW_STATIC)

target_link_libraries(simple glews)

