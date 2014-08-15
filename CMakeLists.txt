project (POGL)
cmake_minimum_required (VERSION 2.6) 

# Root directory for the project
set(ROOT_DIR ${POGL_SOURCE_DIR})

# Default configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Add GUI option
OPTION(POGL_BUILD_EXAMPLES "Build examples" ON)

# Directories
add_subdirectory (pogl)

# If GUI option is set then build examples projects
IF(POGL_BUILD_EXAMPLES)
	set(EXAMPLES_DIR ${ROOT_DIR}/examples)
	add_subdirectory (examples)
ENDIF()
