cmake_minimum_required(VERSION 3.10)
project(C_LR_1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# === Исходные файлы (в папке src/) ===
set(LIB_SOURCES
    src/my_string.c
    src/Dynamic_Array.c
    src/char_type.c
    src/int_type.c
)

set(LIB_HEADERS
    src/my_string.h
    src/Dynamic_Array.h
    src/fieldinfo.h
)

# === Демонстрация ===
add_executable(demo src/main.c ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(demo PRIVATE src)

# === Тесты ===
enable_testing()
if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()