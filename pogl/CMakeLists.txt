# Add OpenGL as library
find_package(OpenGL REQUIRED)

# Create a variable called helloworld_SOURCES containing all .cpp files:
file(GLOB pogl_SOURCES ${PlayGL_SOURCE_DIR}/pogl/src/*.cxx)
if(WIN32)
file(GLOB pogl_win32_SOURCES ${PlayGL_SOURCE_DIR}/pogl/src/Win32*.cxx)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_win32_SOURCES})
elseif(APPLE)
file(GLOB pogl_apple_SOURCES ${PlayGL_SOURCE_DIR}/pogl/src/Apple*.cxx)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_apple_SOURCES})
elseif(UNIX)
file(GLOB pogl_unix_SOURCES ${PlayGL_SOURCE_DIR}/pogl/src/Unix*.cxx)
set(pogl_SOURCES ${pogl_SOURCES} ${pogl_unix_SOURCES})
endif()

include_directories (${PlayGL_SOURCE_DIR}/pogl/include)
include_directories(${OPENGL_INCLUDE_DIRS})

# Create an executable file called helloworld from sources:
add_library(pogl STATIC ${pogl_SOURCES})