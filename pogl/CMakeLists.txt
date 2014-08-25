# Add OpenGL package
find_package(OpenGL REQUIRED)

# Create a variable containing all .cpp files:
file(GLOB pogl_SOURCES_allsubfiles ${ROOT_DIR}/pogl/src/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_SOURCES_allsubfiles})
file(GLOB pogl_SOURCES_allsubfiles ${ROOT_DIR}/pogl/src/uniforms/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_SOURCES_allsubfiles})
file(GLOB pogl_SOURCES_allsubfiles ${ROOT_DIR}/pogl/src/providers/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_SOURCES_allsubfiles})
file(GLOB pogl_SOURCES_allfiles ${ROOT_DIR}/pogl/src/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_SOURCES_allfiles})
if(WIN32)
file(GLOB pogl_win32_SOURCES ${ROOT_DIR}/pogl/src/win32/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_win32_SOURCES})
elseif(APPLE)
file(GLOB pogl_apple_SOURCES ${ROOT_DIR}/pogl/src/apple/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_apple_SOURCES})
elseif(UNIX)
file(GLOB pogl_unix_SOURCES ${ROOT_DIR}/pogl/src/unix/*.cpp)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_unix_SOURCES})
endif()

include_directories (${ROOT_DIR}/pogl/include)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories (${ROOT_DIR}/pogl/src)

# Create Library
if(POGL_BUILD_STATIC)
	add_library(pogl STATIC ${pogl_SOURCES})
else()
	add_library(pogl SHARED ${pogl_SOURCES})
	
	find_package(OpenGL REQUIRED)
	target_link_libraries(pogl ${OPENGL_LIBRARIES})
endif()