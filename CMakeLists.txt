cmake_minimum_required(VERSION 2.8.7)
project(GeoDrill)
# The version number.
set(GeoDrill_VERSION_MAJOR 0)
set(GeoDrill_VERSION_MINOR 0)
set(GeoDrill_VERSION_PATCH 1)
set(GeoDrill_VERSION "${GeoDrill_VERSION_MAJOR}.${GeoDrill_VERSION_MINOR}.${GeoDrill_VERSION_PATCH}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(geodrill)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/geodrill/GeoDrillConfig.h.in"
        "${PROJECT_SOURCE_DIR}/geodrill/GeoDrillConfig.h"
)
# Also dynamically set some documentation settings
configure_file (
        "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
        "${PROJECT_SOURCE_DIR}/geodrill/Doxyfile"
)