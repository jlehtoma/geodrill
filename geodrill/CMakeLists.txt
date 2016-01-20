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

find_package(Doxygen)

if(DOXYGEN_FOUND)

    set(DOXYGEN_INPUT ${SOURCE_FILES})
    set(DOXYGEN_OUTPUT ${PROJECT_BINARY_DIR}/doc)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})


endif(DOXYGEN_FOUND)
