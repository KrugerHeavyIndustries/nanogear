set(nanogear_SRCS
    datetime.cpp
    datetimeformat.cpp
    datetimeformatter.cpp
    numberformatter.cpp
    path.cpp
    file.cpp
    timestamp.cpp
    stringtokenizer.cpp
    timespan.cpp
    directoryiterator.cpp
    directoryresource.cpp
    method.cpp
    textcodec.cpp
    protocol.cpp
    representation.cpp
    resource.cpp
    router.cpp
    status.cpp
    mimetype.cpp
    application.cpp
)

add_library(Nanogear SHARED ${nanogear_SRCS})

set_target_properties(Nanogear PROPERTIES
    VERSION ${NANOGEAR_MAJOR}.${NANOGEAR_MINOR}.${NANOGEAR_PATCH}
    SOVERSION ${NANOGEAR_MAJOR}
)

include_directories(Nanogear ${Libmagic_INCLUDE_DIR})

target_link_libraries(Nanogear PRIVATE ${Libmagic_LIBRARY})

# Install the library
install(TARGETS Nanogear
    LIBRARY DESTINATION lib
)

# Install header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/Nanogear
    FILES_MATCHING PATTERN *.h
)

add_subdirectory(tests)
