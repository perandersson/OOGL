# Create a variable containing all .cpp files:
file(GLOB example_drawcube_SOURCES ${EXAMPLES_DIR}/example_drawcube/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglmath/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_drawcube ${example_drawcube_SOURCES})

# Add link libraries
target_link_libraries(example_drawcube ${OPENGL_LIBRARIES})
target_link_libraries(example_drawcube examples_window)
target_link_libraries(example_drawcube pogl)
target_link_libraries(example_drawcube poglmath)
