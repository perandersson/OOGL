# Create a variable containing all .cpp files:
file(GLOB example_deferredframebuffers_SOURCES ${EXAMPLES_DIR}/example_deferredframebuffers/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_deferredframebuffers ${example_deferredframebuffers_SOURCES})

# Add link libraries
target_link_libraries(example_deferredframebuffers ${OPENGL_LIBRARIES})
target_link_libraries(example_deferredframebuffers examples_window)
target_link_libraries(example_deferredframebuffers pogl)
