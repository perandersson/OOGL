# Create a variable containing all .cpp files:
file(GLOB example_maprangevertexbuffer_SOURCES ${EXAMPLES_DIR}/example_maprangevertexbuffer/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_maprangevertexbuffer ${example_maprangevertexbuffer_SOURCES})

# Add link libraries
target_link_libraries(example_maprangevertexbuffer ${OPENGL_LIBRARIES})
target_link_libraries(example_maprangevertexbuffer examples_window)
target_link_libraries(example_maprangevertexbuffer pogl)
