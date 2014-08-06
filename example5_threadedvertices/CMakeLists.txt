# Create a variable containing all .cpp files:
file(GLOB example5_threadedvertices_SOURCES ${PlayOpenGL_SOURCE_DIR}/example5_threadedvertices/src/*.cpp)
include_directories (${PlayOpenGL_SOURCE_DIR}/pogl/include)
include_directories (${PlayOpenGL_SOURCE_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example5_threadedvertices ${example5_threadedvertices_SOURCES})

# Add link libraries
target_link_libraries(example5_threadedvertices ${OPENGL_LIBRARIES})
target_link_libraries(example5_threadedvertices examples_window)
target_link_libraries(example5_threadedvertices pogl)
