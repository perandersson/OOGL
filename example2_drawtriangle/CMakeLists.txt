# Create a variable containing all .cpp files:
file(GLOB example2_drawtriangle_SOURCES ${PlayOpenGL_SOURCE_DIR}/example2_drawtriangle/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example2_drawtriangle ${example2_drawtriangle_SOURCES})

# Add link libraries
target_link_libraries(example2_drawtriangle ${OPENGL_LIBRARIES})
target_link_libraries(example2_drawtriangle pogl)
