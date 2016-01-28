cmake_minimum_required(VERSION 2.8.11)

find_package(Boost COMPONENTS REQUIRED filesystem program_options system)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

# Set custom library dir
set(GDAL_ROOT ${PROJECT_3RDPARTY_PATH}/gdal)
set(GDAL_LIBRARY_DIR ${GDAL_ROOT}/lib64)

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
    include_directories(${GDAL_INCLUDE_DIR})
else()
    message("-- Looking for GDAL in system")
    find_package(GDAL REQUIRED)
    include_directories(${GDAL_INCLUDE_DIR})
endif()

set(SOURCE_FILES io.cpp ../include/io.h)
add_library(geodrill SHARED ${SOURCE_FILES})
target_link_libraries(geodrill ${Boost_LIBRARIES} ${GDAL_LIBRARY})

set(SOURCE_FILES main.cpp file_utils.cpp ../include/GeoDrillConfig.h ../include/file_utils.h)
add_executable(geodrill-cli ${SOURCE_FILES})
target_link_libraries(geodrill-cli geodrill ${Boost_LIBRARIES})
