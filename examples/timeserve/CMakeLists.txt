include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/src/connectors)
add_executable(timeserve main.cpp)
target_link_libraries(timeserve NanogearMongooseHTTPConnector Nanogear)
