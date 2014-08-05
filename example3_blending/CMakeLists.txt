# Create a variable containing all .cpp files:
file(GLOB example3_blending_SOURCES ${PlayOpenGL_SOURCE_DIR}/example3_blending/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)
include_directories (${PlayOpenGL_SOURCE_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example3_blending ${example3_blending_SOURCES})

# Add link libraries
target_link_libraries(example3_blending ${OPENGL_LIBRARIES})
target_link_libraries(example3_blending examples_window)
target_link_libraries(example3_blending pogl)
