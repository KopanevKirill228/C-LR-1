// tests/test_runner.c
#include "test_macros.h"  // ← Подключаем макросы
#include "my_string.h"
#include "fieldinfo.h"
#include <windows.h>

// Определение глобальных переменных (теперь extern работает)
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

// Прототипы тестов
void test_string_create(void);
void test_string_concat(void);
void test_string_substring(void);
void test_string_find(void);
void test_fieldinfo(void);

int main(void) {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    printf("\n========================================\n");
    printf("   AUTOMATED TEST SUITE - VARIANT 7\n");
    printf("========================================\n\n");

    printf(">>> String_Create / String_Destroy\n");
    test_string_create();
    printf("\n");

    printf(">>> String_Concat\n");
    test_string_concat();
    printf("\n");

    printf(">>> String_Substring\n");
    test_string_substring();
    printf("\n");

    printf(">>> String_Find\n");
    test_string_find();
    printf("\n");

    printf(">>> FieldInfo\n");
    test_fieldinfo();
    printf("\n");

    printf("========================================\n");
    printf("  Всего: %d | Пройдено: %d | Провалено: %d\n", 
           tests_run, tests_passed, tests_failed);
    printf("========================================\n");

    if (tests_failed == 0) {
        printf("  ✓ ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("  ✗ SOME TESTS FAILED\n");
        return 1;
    }
}