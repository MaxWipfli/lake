set(LAKE_TEST_NAMES
    test_array
    test_extras
    test_fixed_array
    test_hash
    test_optional
    test_span
    test_string_view
    test_unique_ptr
    test_vector
)

include(GoogleTest)

foreach (name ${LAKE_TEST_NAMES})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} lake GTest::gtest_main)
    gtest_discover_tests(${name})
endforeach()
