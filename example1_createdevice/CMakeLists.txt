# Create a variable containing all .cpp files:
file(GLOB example1_createdevice_SOURCES ${PlayOpenGL_SOURCE_DIR}/example1_createdevice/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example1_createdevice ${example1_createdevice_SOURCES})

# Add link libraries
target_link_libraries(example1_createdevice ${OPENGL_LIBRARIES})
target_link_libraries(example1_createdevice pogl)
