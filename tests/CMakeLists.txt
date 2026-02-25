# tests/CMakeLists.txt
set(TEST_SOURCES
    test_runner.c
    test_string.c
    test_fieldinfo.c
)

# Исходные файлы библиотеки (чтобы тесты видели функции)
set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/my_string.c
    ${CMAKE_SOURCE_DIR}/src/Vector.c
    ${CMAKE_SOURCE_DIR}/src/char_type.c
)

add_executable(run_tests ${TEST_SOURCES} ${LIB_SOURCES})

# Путь к заголовкам
target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

add_test(NAME StringTests COMMAND run_tests)
set_tests_properties(StringTests PROPERTIES 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    PASS_REGULAR_EXPRESSION "ALL TESTS PASSED"
)