# Create a variable containing all .cpp files:
file(GLOB examples_window_SOURCES_allsubfiles ${EXAMPLES_DIR}/examples_window/src/*.cpp)
set(examples_window_SOURCES ${examples_window_SOURCES} ${examples_window_SOURCES_allsubfiles})
file(GLOB examples_window_SOURCES_allfiles ${EXAMPLES_DIR}/examples_window/src/*.cpp)
set(examples_window_SOURCES ${examples_window_SOURCES} ${examples_window_SOURCES_allfiles})
if(WIN32)
file(GLOB examples_window_win32_SOURCES ${EXAMPLES_DIR}/examples_window/src/win32/*.cpp)
set(examples_window_SOURCES ${examples_window_SOURCES} ${examples_window_win32_SOURCES})
elseif(APPLE)
file(GLOB examples_window_apple_SOURCES ${EXAMPLES_DIR}/examples_window/src/apple/*.cpp)
set(examples_window_SOURCES ${examples_window_SOURCES} ${examples_window_apple_SOURCES})
elseif(UNIX)
file(GLOB examples_window_unix_SOURCES ${EXAMPLES_DIR}/examples_window/src/unix/*.cpp)
set(examples_window_SOURCES ${examples_window_SOURCES} ${examples_window_unix_SOURCES})
endif()

include_directories (${ROOT_DIR}/pogl/include)
include_directories (${EXAMPLES_DIR}/examples_window/include)

# Create Library
add_library(examples_window STATIC ${examples_window_SOURCES})
