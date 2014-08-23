# Create a variable containing all .cpp files:
file(GLOB example_threadlineardepthmap_SOURCES ${EXAMPLES_DIR}/example_threadlineardepthmap/src/*.cpp)
include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglmath/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create an executable file from sources
add_executable(example_threadlineardepthmap ${example_threadlineardepthmap_SOURCES})

# Add link libraries
target_link_libraries(example_threadlineardepthmap ${OPENGL_LIBRARIES})
target_link_libraries(example_threadlineardepthmap examples_window)
target_link_libraries(example_threadlineardepthmap pogl)
target_link_libraries(example_threadlineardepthmap poglmath)