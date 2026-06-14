cmake_minimum_required(VERSION 3.10)
project(nanogear CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
include(NanogearVersion)

message(STATUS "This is Nanogear ${NANOGEAR_MAJOR}.${NANOGEAR_MINOR}.${NANOGEAR_PATCH}")

option(WITH_EXAMPLES "WITH_EXAMPLES" OFF)
option(WITH_TESTS "WITH_TESTS" ON)

find_package(Libmagic REQUIRED)

if (${WITH_TESTS})
    include(FetchContent)
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        OVERRIDE_FIND_PACKAGE
    )
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(doctest)
    enable_testing()
endif (${WITH_TESTS})

#
# Build documentation if doxygen and graphviz are found
#
find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message(STATUS "Use the 'apidocs' target to build documentation")
    add_subdirectory(docs)
else (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message(STATUS "You will need both doxygen and graphviz 'dot' to build documentation")
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)


#
# Pedantic mode
# Enable all warnings and treat them as errors
#
if (${PEDANTIC})
    if (${CMAKE_COMPILER_IS_GNUCC} OR ${CMAKE_COMPILER_IS_GNUCXX})
        message(STATUS "-- Pedantic mode enabled for GCC compiler")
        add_definitions(-W -Wall -pedantic -Werror)
    else (${CMAKE_COMPILER_IS_GNUCC} OR ${CMAKE_COMPILER_IS_GNUCXX})
        message(STATUS "-- Unable to find a supported compiler")
    endif (${CMAKE_COMPILER_IS_GNUCC} OR ${CMAKE_COMPILER_IS_GNUCXX})
else (${PEDANTIC})
    message(STATUS "Pedantic mode is NOT enabled")
    message(STATUS "  If you are a developer you are strongly encouraged to activate")
    message(STATUS "  the pedantic mode")
endif (${PEDANTIC})


#
# Continue build
#
add_subdirectory(src)

if (${WITH_EXAMPLES})
    add_subdirectory(examples)
endif (${WITH_EXAMPLES})
