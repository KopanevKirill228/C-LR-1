// tests/test_macros.h
#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Счётчики
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Цвета для вывода (если консоль поддерживает)
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

// Запуск группы тестов
#define TEST_GROUP(name) \
    printf("\n┌─► %s\n", name); \
    fflush(stdout);

// Завершение группы
#define TEST_GROUP_END() \
    fflush(stdout);

// Основной макрос теста
#define TEST_RUN(description, condition) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("│  ◦ %s ... %s✓ PASS%s\n", description, COLOR_GREEN, COLOR_RESET); \
    } else { \
        tests_failed++; \
        printf("│  ◦ %s ... %s✗ FAIL%s\n", description, COLOR_RED, COLOR_RESET); \
        fprintf(stderr, "    [FAILED] %s:%d\n", __FILE__, __LINE__); \
    } \
    fflush(stdout); \
} while(0)

// Проверка на NULL
#define ASSERT_NOT_NULL(ptr, msg) \
    TEST_RUN(msg, (ptr) != NULL)

// Проверка на равенство
#define ASSERT_EQ(expected, actual, msg) \
    TEST_RUN(msg, (expected) == (actual))

// Проверка строки
#define ASSERT_STR_EQ(expected, actual, msg) \
    TEST_RUN(msg, strcmp((expected), (actual)) == 0)

// Вывод статистики
#define PRINT_STATS() do { \
    printf("\n╔════════════════════════════════════╗\n"); \
    printf("║         TEST RESULTS                 ║\n"); \
    printf("╠════════════════════════════════════╣\n"); \
    printf("║  Total:    %-4d                    ║\n", tests_run); \
    printf("║  Passed:   %-4d ✓                  ║\n", tests_passed); \
    printf("║  Failed:   %-4d ✗                  ║\n", tests_failed); \
    printf("╚════════════════════════════════════╝\n"); \
    if (tests_failed == 0) { \
        printf("%s🎉 ALL TESTS PASSED!%s\n", COLOR_GREEN, COLOR_RESET); \
    } else { \
        printf("%s⚠️  SOME TESTS FAILED%s\n", COLOR_RED, COLOR_RESET); \
    } \
} while(0)

#endif