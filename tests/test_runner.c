#include <stdio.h>
#include <windows.h>  // Для кодировки консоли
#include "test_macros.h"

// Прототипы тестов из test_core.c
void test_fieldinfo_basics(void);
void test_vector_basics(void);
void test_vector_advanced(void);

// Прототипы тестов из test_string.c
void test_string_create_destroy(void);
void test_string_concat(void);
void test_string_substring(void);
void test_string_find(void);
void test_string_utils(void);

// Глобальные переменные статистики (определены здесь)
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

int main(void) {
    // Настройка кодировки для русского текста
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║   AUTOMATED TEST SUITE — VARIANT 7     ║\n");
    printf("║   Polymorphic Collection: STRING       ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    // === Базовые компоненты ===
    test_fieldinfo_basics();
    test_vector_basics();
    test_vector_advanced();
    
    // === АТД Строка ===
    test_string_create_destroy();
    test_string_concat();
    test_string_substring();
    test_string_find();
    test_string_utils();
    
    // === Итоговый отчёт ===
    TEST_REPORT();
    
    return (tests_failed == 0) ? 0 : 1;
}