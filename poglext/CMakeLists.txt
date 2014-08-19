# Create a variable containing all .cpp files:
file(GLOB pogl_SOURCES ${ROOT_DIR}/poglext/src/*.cpp)

include_directories (${ROOT_DIR}/pogl/include)
include_directories (${ROOT_DIR}/poglext/include)

# Create Library
if(POGL_BUILD_STATIC)
	add_library(poglext STATIC ${pogl_SOURCES})
else()
	add_library(poglext SHARED ${pogl_SOURCES})
	target_link_libraries(poglext pogl)
endif()