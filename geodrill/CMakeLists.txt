cmake_minimum_required(VERSION 2.8.11)
include_directories(${GeoDrill_SOURCE_DIR}/geodrill)
link_directories(${GeoDrill_BINARY_DIR}/geodrill)

find_package(Boost COMPONENTS REQUIRED program_options)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

# Set custom library dir
set(PROJECT_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)
set(GDAL_ROOT ${PROJECT_LIBRARY_PATH}/gdal)

# Try looking for custom build of GDAL in GeoDrill/lib, in practice this means GDAL > 2.0 which is needed
# for some functionality. If it's not around, try looking for a regular system-wide installation of GDAL.
find_library(GDAL_LIBRARY NAMES gdal libgdal PATHS ${GDAL_LIBRARY_DIR} NO_DEFAULT_PATH)

if (GDAL_LIBRARY)
    set(GDAL_FOUND TRUE)
else(GDAL_LIBRARY)
    set(GDAL_FOUND FALSE)
endif(GDAL_LIBRARY)

if (GDAL_FOUND)
    message("-- Using project-sepcific GDAL")
    message("-- Found GDAL: ${GDAL_LIBRARY}")
    set(GDAL_INCLUDE_DIR ${GDAL_ROOT}/include)
    set(GDAL_LIBRARY_DIR ${GDAL_ROOT}/lib64)
    include_directories(${GDAL_INCLUDE_DIR})
else()
    message("-- Looking for GDAL in system")
    find_package(GDAL REQUIRED)
    include_directories(${GDAL_INCLUDE_DIR})
endif()

set(SOURCE_FILES main.cpp io.cpp io.h)
add_executable(geodrill ${SOURCE_FILES})
target_link_libraries(geodrill ${Boost_LIBRARIES} ${GDAL_LIBRARY})
