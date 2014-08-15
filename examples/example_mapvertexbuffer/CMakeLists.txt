# Create a variable containing all .cpp files:
file(GLOB example_mapvertexbuffer_SOURCES ${EXAMPLES_DIR}/example_mapvertexbuffer/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_mapvertexbuffer ${example_mapvertexbuffer_SOURCES})

# Add link libraries
target_link_libraries(example_mapvertexbuffer ${OPENGL_LIBRARIES})
target_link_libraries(example_mapvertexbuffer examples_window)
target_link_libraries(example_mapvertexbuffer pogl)
