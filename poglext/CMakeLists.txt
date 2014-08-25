# Create a variable containing all .cpp files:
file(GLOB poglext_SOURCES ${ROOT_DIR}/poglext/src/*.cpp)

include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglext/include)
include_directories (${ROOT_DIR}/poglmath/include)

# Create Library
if(POGL_BUILD_STATIC)
	add_library(poglext STATIC ${poglext_SOURCES})
else()
	add_library(poglext SHARED ${poglext_SOURCES})
	target_link_libraries(poglext pogl)
	target_link_libraries(poglext poglmath)
endif()