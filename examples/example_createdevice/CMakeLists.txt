# Create a variable containing all .cpp files:
file(GLOB example_createdevice_SOURCES ${EXAMPLES_DIR}/example_createdevice/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_createdevice ${example_createdevice_SOURCES})

# Add link libraries
target_link_libraries(example_createdevice ${OPENGL_LIBRARIES})
target_link_libraries(example_createdevice pogl)
