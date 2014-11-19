set(mongoose_SRCS mongoose.c)

include_directories(${CMAKE_SOURCE_DIR}/src/core)

add_library(NanogearMongooseHTTPConnector SHARED ${mongoose_SRCS})

set_target_properties(NanogearMongooseHTTPConnector PROPERTIES
    VERSION ${NANOGEAR_MAJOR}.${NANOGEAR_MINOR}.${NANOGEAR_PATCH}
    SOVERSION ${NANOGEAR_MAJOR}
)

target_link_libraries(NanogearMongooseHTTPConnector Nanogear)

# Install the library
install(TARGETS NanogearMongooseHTTPConnector
    LIBRARY DESTINATION lib
)

# Install header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/Nanogear/Connectors/Mongoose
    FILES_MATCHING PATTERN *.h
)

#add_subdirectory(tests)
