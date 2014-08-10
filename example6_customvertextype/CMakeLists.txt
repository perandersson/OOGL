# Create a variable containing all .cpp files:
file(GLOB example6_customvertextype_SOURCES ${PlayOpenGL_SOURCE_DIR}/example6_customvertextype/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)
include_directories (${PlayOpenGL_SOURCE_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example6_customvertextype ${example6_customvertextype_SOURCES})

# Add link libraries
target_link_libraries(example6_customvertextype ${OPENGL_LIBRARIES})
target_link_libraries(example6_customvertextype examples_window)
target_link_libraries(example6_customvertextype pogl)
