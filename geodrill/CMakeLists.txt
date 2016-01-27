include_directories(${GeoDrill_SOURCE_DIR}/geodrill)
link_directories(${GeoDrill_BINARY_DIR}/geodrill)

find_package(Boost COMPONENTS REQUIRED program_options)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

find_package(GDAL)
if (GDAL_FOUND)
    include_directories(${GDAL_INCLUDE_DIR})
endif ()

set(SOURCE_FILES main.cpp io.cpp io.h)
add_executable(geodrill ${SOURCE_FILES})
target_link_libraries(geodrill ${Boost_LIBRARIES} ${GDAL_LIBRARY})
