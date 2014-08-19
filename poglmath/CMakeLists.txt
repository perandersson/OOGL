# Create a variable containing all .cpp files:
file(GLOB poglmath_SOURCES ${ROOT_DIR}/poglmath/src/*.cpp)

include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglmath/include)

# Create Library
if(POGL_BUILD_STATIC)
	add_library(poglmath STATIC ${poglmath_SOURCES})
else()
	add_library(poglmath SHARED ${poglmath_SOURCES})
	target_link_libraries(poglmath pogl)
endif()