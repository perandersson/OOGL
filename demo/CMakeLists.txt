# Create a variable called helloworld_SOURCES containing all .cpp files:
file(GLOB demo_SOURCES ${playogl_SOURCE_DIR}/demo/src/*.cxx)
include_directories (${playogl_SOURCE_DIR}/pogl/include)

# Find packages
find_package(OpenGL REQUIRED)

# Create an executable file called demo from sources
add_executable(demo ${demo_SOURCES})

# Add link libraries
target_link_libraries(demo ${OPENGL_LIBRARIES})
target_link_libraries(demo pogl)
