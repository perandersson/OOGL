# Create a variable containing all .cpp files:
file(GLOB example7_framebuffers ${PlayOpenGL_SOURCE_DIR}/example7_framebuffers/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)
include_directories (${PlayOpenGL_SOURCE_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example7_framebuffers ${example7_framebuffers})

# Add link libraries
target_link_libraries(example7_framebuffers ${OPENGL_LIBRARIES})
target_link_libraries(example7_framebuffers examples_window)
target_link_libraries(example7_framebuffers pogl)
