if (${WITH_TESTS})
    add_executable(nanogear_tests
        test_representation.cpp
        test_clientinfo.cpp
    )

    target_include_directories(nanogear_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_link_libraries(nanogear_tests PRIVATE Nanogear doctest::doctest_with_main)

    add_test(NAME nanogear_tests COMMAND nanogear_tests -ltc)
endif (${WITH_TESTS})
