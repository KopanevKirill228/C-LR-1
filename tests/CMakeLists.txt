# Исполняемый файл для тестов
add_executable(run_tests
    test_runner.c
    test_core.c
    test_string.c
    ../src/my_string.c
    ../src/dynamic_array.c
    ../src/char_type.c
    ../src/int_type.c
    ../src/fieldinfo.h
    ../src/tokenizer.c
    ../src/string_type.c 
    test_tokenizer.c
)

# Линковка
target_link_libraries(run_tests)

# Включение в CTest
enable_testing()
add_test(NAME run_tests COMMAND run_tests)

# Флаги компиляции (опционально)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(run_tests PRIVATE -g -Wall -Wextra)
endif()