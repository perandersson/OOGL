# Create a variable containing all .cpp files:
file(GLOB example_drawtriangle_SOURCES ${EXAMPLES_DIR}/example_drawtriangle/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_drawtriangle ${example_drawtriangle_SOURCES})

# Add link libraries
target_link_libraries(example_drawtriangle ${OPENGL_LIBRARIES})
target_link_libraries(example_drawtriangle examples_window)
target_link_libraries(example_drawtriangle pogl)
