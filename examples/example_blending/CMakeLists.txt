# Create a variable containing all .cpp files:
file(GLOB example_blending_SOURCES ${EXAMPLES_DIR}/example_blending/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_blending ${example_blending_SOURCES})

# Add link libraries
target_link_libraries(example_blending ${OPENGL_LIBRARIES})
target_link_libraries(example_blending examples_window)
target_link_libraries(example_blending pogl)
