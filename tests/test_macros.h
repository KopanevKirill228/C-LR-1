#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Глобальная статистика
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

// === ВИЗУАЛЬНЫЕ МАКРОСЫ ===

// Заголовок группы тестов
#define TEST_GROUP(title) \
    do { \
        printf("\n┌─► %s\n", title); \
        printf("│\n"); \
    } while(0)

// Тест с описанием и результатом
#define TEST_RUN(description, condition) \
    do { \
        tests_run++; \
        printf("│  ◦ %s ... ", description); \
        if (condition) { \
            tests_passed++; \
            printf("✓ PASS\n"); \
        } else { \
            tests_failed++; \
            printf("✗ FAIL\n"); \
        } \
    } while(0)

// Тест с показом входных/выходных данных
#define TEST_SHOW(description, condition, input, expected, actual) \
    do { \
        tests_run++; \
        printf("│  ◦ %s\n", description); \
        printf("│     Input:    %s\n", input); \
        printf("│     Expected: %s\n", expected); \
        printf("│     Actual:   %s ... ", actual); \
        if (condition) { \
            tests_passed++; \
            printf("✓\n"); \
        } else { \
            tests_failed++; \
            printf("✗ FAIL\n"); \
        } \
    } while(0)

// Разделитель между тестами
#define TEST_SEPARATOR() printf("│\n")

// Завершение группы
#define TEST_GROUP_END() printf("└─► Done\n")

// Итоговый отчёт
#define TEST_REPORT() \
    do { \
        printf("\n╔════════════════════════════════════╗\n"); \
        printf("║         TEST RESULTS                 ║\n"); \
        printf("╠════════════════════════════════════╣\n"); \
        printf("║  Total:    %-3d                     ║\n", tests_run); \
        printf("║  Passed:   %-3d ✓                   ║\n", tests_passed); \
        printf("║  Failed:   %-3d ✗                   ║\n", tests_failed); \
        printf("╚════════════════════════════════════╝\n"); \
        if (tests_failed == 0) { \
            printf("\n🎉 ALL TESTS PASSED!\n\n"); \
        } else { \
            printf("\n⚠️  SOME TESTS FAILED\n\n"); \
        } \
    } while(0)

#endif