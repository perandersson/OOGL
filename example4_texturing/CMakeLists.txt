# Create a variable containing all .cpp files:
file(GLOB example4_texturing_SOURCES ${PlayOpenGL_SOURCE_DIR}/example4_texturing/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)
include_directories (${PlayOpenGL_SOURCE_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

set(CMAKE_CURRENT_BINARY_DIR ${PlayOpenGL_SOURCE_DIR}/example4_texturing)

# Create an executable file from sources
add_executable(example4_texturing ${example4_texturing_SOURCES})

# Add link libraries
target_link_libraries(example4_texturing ${OPENGL_LIBRARIES})
target_link_libraries(example4_texturing examples_window)
target_link_libraries(example4_texturing pogl)
