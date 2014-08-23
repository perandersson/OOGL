# Create a variable containing all .cpp files:
file(GLOB example_lineardepthmap_SOURCES ${EXAMPLES_DIR}/example_lineardepthmap/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglmath/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_lineardepthmap ${example_lineardepthmap_SOURCES})

# Add link libraries
target_link_libraries(example_lineardepthmap ${OPENGL_LIBRARIES})
target_link_libraries(example_lineardepthmap examples_window)
target_link_libraries(example_lineardepthmap pogl)
target_link_libraries(example_lineardepthmap poglmath)