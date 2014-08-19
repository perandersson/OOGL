# Create a variable containing all .cpp files:
file(GLOB pogl_SOURCES ${ROOT_DIR}/poglext/src/*.cpp)

include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglext/include)

# Create Library
add_library(poglext STATIC ${pogl_SOURCES})