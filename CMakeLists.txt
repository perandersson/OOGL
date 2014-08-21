project (POGL)
cmake_minimum_required (VERSION 2.6) 

# Root directory for the project
set(ROOT_DIR ${POGL_SOURCE_DIR})

# Default configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Add GUI option
OPTION(POGL_BUILD_EXAMPLES "Build examples" ON)
OPTION(POGL_ENABLE_SSE2 "Enable Enhanced Instruction Set" ON)
OPTION(POGL_BUILD_STATIC "Static Build" ON)

# Add definitions

IF(MSVC)
	ADD_DEFINITIONS(/fp:fast)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS=1)
	ADD_DEFINITIONS(-DPOGL_BUILD=1)
ENDIF()

IF(MINGW)
	ADD_DEFINITIONS(-std=c++11)
ENDIF()

IF(POGL_BUILD_STATIC)
	IF(MSVC)
		ADD_DEFINITIONS(-DPOGL_STATIC=1)
	ENDIF()
ENDIF()

IF(POGL_ENABLE_SSE2)
	IF(MSVC)
		ADD_DEFINITIONS(/arch:SSE2)
		ADD_DEFINITIONS(-DPOGL_ENHANCED_INSTRUCTION_SET=1)
	ENDIF()
ENDIF()

# Directories
add_subdirectory (pogl)
add_subdirectory (poglext)
add_subdirectory (poglmath)

# If GUI option is set then build examples projects
IF(POGL_BUILD_EXAMPLES)
	set(EXAMPLES_DIR ${ROOT_DIR}/examples)
	add_subdirectory (examples)
ENDIF()
