# Create a variable containing all .cpp files:
file(GLOB example_customvertextype_SOURCES ${EXAMPLES_DIR}/example_customvertextype/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_customvertextype ${example_customvertextype_SOURCES})

# Add link libraries
target_link_libraries(example_customvertextype ${OPENGL_LIBRARIES})
target_link_libraries(example_customvertextype examples_window)
target_link_libraries(example_customvertextype pogl)
