set(TEST_SOURCES
    test_runner.c
    test_core.c
    test_string.c
)

# Исходные файлы библиотеки (чтобы тесты видели реализацию)
set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/my_string.c
    ${CMAKE_SOURCE_DIR}/src/Vector.c
    ${CMAKE_SOURCE_DIR}/src/char_type.c
)

add_executable(run_tests ${TEST_SOURCES} ${LIB_SOURCES})

target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

# Регистрация для CTest
add_test(NAME AllTests COMMAND run_tests)
set_tests_properties(AllTests PROPERTIES 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    PASS_REGULAR_EXPRESSION "ALL TESTS PASSED"
)