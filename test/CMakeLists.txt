set(LAKE_TEST_NAMES
    test_array
    test_extras
    test_optional
    test_span
    test_unique_ptr
    test_vector
)

include(GoogleTest)

foreach (name ${LAKE_TEST_NAMES})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} lake GTest::gtest_main)
    target_include_directories(${name} PRIVATE ../include)
    gtest_discover_tests(${name})
endforeach()
