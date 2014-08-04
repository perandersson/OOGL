# http://brunoabinader.com/?p=29
# Thank you for this! :-)

project (PlayOpenGL)
cmake_minimum_required (VERSION 3.0) 

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (ie. MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${PlayOpenGL_SOURCE_DIR}/cmake/modules/ ${CMAKE_MODULE_PATH})

# Default configuration types
set(CMAKE_CONFIGURATION_TYPES Debug;Release)

#
# Directories
#

add_subdirectory (pogl)
add_subdirectory (examples_window)
add_subdirectory (example1_createdevice)
add_subdirectory (example2_drawtriangle)

