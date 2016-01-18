include_directories(${GeoDrill_SOURCE_DIR}/geodrill)
link_directories(${GeoDrill_BINARY_DIR}/geodrill)

set(SOURCE_FILES main.cpp)
add_executable(geodrill ${SOURCE_FILES})
#target_link_libraries(cdemo m) #link the math library
