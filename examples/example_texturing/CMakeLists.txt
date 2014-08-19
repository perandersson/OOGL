# Create a variable containing all .cpp files:
file(GLOB example_texturing_SOURCES ${EXAMPLES_DIR}/example_texturing/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglext/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_texturing ${example_texturing_SOURCES})

# Add link libraries
target_link_libraries(example_texturing ${OPENGL_LIBRARIES})
target_link_libraries(example_texturing examples_window)
target_link_libraries(example_texturing pogl)
target_link_libraries(example_texturing poglext)
