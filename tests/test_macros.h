#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Цвета для вывода
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
#define TEST(condition, description) do { \
    if (condition) { \
        printf("│  ◦ %s ... %s✓ PASS%s\n", description, COLOR_GREEN, COLOR_RESET); \
    } else { \
        printf("│  ◦ %s ... %s✗ FAIL%s\n", description, COLOR_RED, COLOR_RESET); \
        fprintf(stderr, "    [FAILED] %s:%d\n", __FILE__, __LINE__); \
    } \
    fflush(stdout); \
} while(0)

// Проверка на NULL
#define ASSERT_NOT_NULL(ptr, msg) \
    TEST((ptr) != NULL, msg)

// Проверка на равенство
#define ASSERT_EQ(expected, actual, msg) \
    TEST((expected) == (actual), msg)

// Пустая статистика
#define PRINT_STATS() do { \
    printf("\n╔════════════════════════════════════╗\n"); \
    printf("║         TESTS COMPLETED             ║\n"); \
    printf("╚════════════════════════════════════╝\n"); \
} while(0)

#endif