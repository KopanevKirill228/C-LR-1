// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_string.h"
#include "Vector.h"
#include "fieldinfo.h"

/* ============================================================================
 * Счётчик тестов
 * ========================================================================== */
static int tests_passed = 0;
static int tests_failed = 0;

// Макрос для проверки условий
#define TEST_ASSERT(condition, test_name) \
    do { \
        if (condition) { \
            printf("  ✓ PASS: %s\n", test_name); \
            tests_passed++; \
        } else { \
            printf("  ✗ FAIL: %s\n", test_name); \
            tests_failed++; \
        } \
    } while (0)

/* ============================================================================
 * Тест 1: Создание и уничтожение строки
 * ========================================================================== */
void test_string_create_destroy(void) {
    printf("\n=== Тест: Создание и уничтожение ===\n");
    
    // Нормальное создание
    String* s1 = String_Create("Hello");
    TEST_ASSERT(s1 != NULL, "String_Create(\"Hello\") != NULL");
    TEST_ASSERT(String_Length(s1) == 5, "Длина \"Hello\" == 5");
    String_Destroy(s1);
    
    // Пустая строка
    String* s2 = String_Create("");
    TEST_ASSERT(s2 != NULL, "String_Create(\"\") != NULL");
    TEST_ASSERT(String_Length(s2) == 0, "Длина пустой строки == 0");
    String_Destroy(s2);
    
    // NULL вход (должна создать пустую строку)
    String* s3 = String_Create(NULL);
    TEST_ASSERT(s3 != NULL, "String_Create(NULL) создаёт пустую строку");
    String_Destroy(s3);
    
    // Двойное уничтожение (защита от краша)
    String* s4 = String_Create("Test");
    String_Destroy(s4);
    String_Destroy(s4);  // Не должно падать
    TEST_ASSERT(true, "Двойной String_Destroy безопасен");
}

/* ============================================================================
 * Тест 2: Конкатенация (Вариант 7)
 * ========================================================================== */
void test_string_concat(void) {
    printf("\n=== Тест: Конкатенация ===\n");
    
    String* s1 = String_Create("Hello, ");
    String* s2 = String_Create("World!");
    String* result = String_Concat(s1, s2);
    
    TEST_ASSERT(result != NULL, "String_Concat не возвращает NULL");
    TEST_ASSERT(String_Length(result) == 13, "Длина конкатенации == 13");
    
    const char* c_str = String_ToCString(result);
    TEST_ASSERT(c_str != NULL, "String_ToCString != NULL");
    TEST_ASSERT(strcmp(c_str, "Hello, World!") == 0, "Содержимое конкатенации верно");
    
    String_Destroy(s1);
    String_Destroy(s2);
    String_Destroy(result);
    
    // Конкатенация с пустой строкой
    String* empty = String_Create("");
    String* s3 = String_Create("Test");
    String* r1 = String_Concat(empty, s3);
    String* r2 = String_Concat(s3, empty);
    
    TEST_ASSERT(strcmp(String_ToCString(r1), "Test") == 0, "Конкатенация с пустой (слева)");
    TEST_ASSERT(strcmp(String_ToCString(r2), "Test") == 0, "Конкатенация с пустой (справа)");
    
    String_Destroy(empty);
    String_Destroy(s3);
    String_Destroy(r1);
    String_Destroy(r2);
    
    // Конкатенация с NULL
    String* r3 = String_Concat(NULL, s3);
    TEST_ASSERT(r3 == NULL, "Конкатенация с NULL возвращает NULL");
}

/* ============================================================================
 * Тест 3: Подстрока (Вариант 7)
 * ========================================================================== */
void test_string_substring(void) {
    printf("\n=== Тест: Подстрока ===\n");
    
    String* s = String_Create("Hello, World!");
    
    // Нормальная подстрока
    String* sub1 = String_Substring(s, 0, 5);
    TEST_ASSERT(sub1 != NULL, "Подстрока [0, 5) != NULL");
    TEST_ASSERT(strcmp(String_ToCString(sub1), "Hello") == 0, "Подстрока \"Hello\" верна");
    String_Destroy(sub1);
    
    // Подстрока из середины
    String* sub2 = String_Substring(s, 7, 12);
    TEST_ASSERT(strcmp(String_ToCString(sub2), "World") == 0, "Подстрока \"World\" верна");
    String_Destroy(sub2);
    
    // Граничные случаи
    String* sub3 = String_Substring(s, 0, String_Length(s));
    TEST_ASSERT(strcmp(String_ToCString(sub3), "Hello, World!") == 0, "Полная подстрока == оригинал");
    String_Destroy(sub3);
    
    // Пустая подстрока
    String* sub4 = String_Substring(s, 5, 5);
    TEST_ASSERT(String_Length(sub4) == 0, "Пустая подстрока [5, 5)");
    String_Destroy(sub4);
    
    // Некорректные индексы (должно возвращать NULL)
    String* sub5 = String_Substring(s, 10, 5);  // start > end
    TEST_ASSERT(sub5 == NULL, "Подстрока start > end возвращает NULL");
    
    String* sub6 = String_Substring(s, 0, 100);  // end > length
    TEST_ASSERT(sub6 == NULL, "Подстрока end > length возвращает NULL");
    
    String* sub7 = String_Substring(NULL, 0, 5);  // NULL строка
    TEST_ASSERT(sub7 == NULL, "Подстрока NULL строки возвращает NULL");
    
    String_Destroy(s);
}

/* ============================================================================
 * Тест 4: Поиск подстроки (Вариант 7)
 * ========================================================================== */
void test_string_find(void) {
    printf("\n=== Тест: Поиск подстроки ===\n");
    
    String* text = String_Create("Hello, Hello, HELLO!");
    String* pattern = String_Create("Hello");
    
    // Поиск с учетом регистра
    size_t count_sensitive = 0;
    int* indices1 = String_Find(text, pattern, true, &count_sensitive);
    
    TEST_ASSERT(indices1 != NULL, "Поиск с учетом регистра нашел совпадения");
    TEST_ASSERT(count_sensitive == 2, "Найдено 2 совпадения (с учетом регистра)");
    if (indices1) {
        TEST_ASSERT(indices1[0] == 0, "Первое вхождение на позиции 0");
        TEST_ASSERT(indices1[1] == 7, "Второе вхождение на позиции 7");
        free(indices1);
    }
    
    // Поиск без учета регистра
    size_t count_insensitive = 0;
    int* indices2 = String_Find(text, pattern, false, &count_insensitive);
    
    TEST_ASSERT(indices2 != NULL, "Поиск без учета регистра нашел совпадения");
    TEST_ASSERT(count_insensitive == 3, "Найдено 3 совпадения (без учета регистра)");
    if (indices2) {
        TEST_ASSERT(indices2[2] == 14, "Третье вхождение (HELLO) на позиции 14");
        free(indices2);
    }
    
    // Поиск несуществующей подстроки
    String* not_found = String_Create("XYZ");
    size_t count_none = 0;
    int* indices3 = String_Find(text, not_found, true, &count_none);
    
    TEST_ASSERT(indices3 == NULL, "Поиск несуществующей подстроки возвращает NULL");
    TEST_ASSERT(count_none == 0, "Счетчик = 0 при отсутствии совпадений");
    
    // Поиск с NULL параметрами
    int* indices4 = String_Find(NULL, pattern, true, &count_none);
    TEST_ASSERT(indices4 == NULL, "Поиск в NULL строке возвращает NULL");
    
    String_Destroy(text);
    String_Destroy(pattern);
    String_Destroy(not_found);
}

/* ============================================================================
 * Тест 5: Полиморфизм (FieldInfo)
 * ========================================================================== */
void test_polymorphism(void) {
    printf("\n=== Тест: Полиморфизм (FieldInfo) ===\n");
    
    const FieldInfo* char_info1 = GetCharFieldInfo();
    const FieldInfo* char_info2 = GetCharFieldInfo();
    
    TEST_ASSERT(char_info1 != NULL, "GetCharFieldInfo() != NULL");
    TEST_ASSERT(char_info1 == char_info2, "Ленивая инициализация (один экземпляр)");
    TEST_ASSERT(char_info1->element_size == sizeof(char), "element_size == 1");
    TEST_ASSERT(char_info1->copy != NULL, "Функция copy определена");
    TEST_ASSERT(char_info1->destroy != NULL, "Функция destroy определена");
    TEST_ASSERT(char_info1->compare != NULL, "Функция compare определена");
}

/* ============================================================================
 * Запуск всех тестов
 * ========================================================================== */
void run_all_tests(void) {
    printf("\n========================================");
    printf("\n       ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ");
    printf("\n========================================\n");
    
    tests_passed = 0;
    tests_failed = 0;
    
    test_string_create_destroy();
    test_string_concat();
    test_string_substring();
    test_string_find();
    test_polymorphism();
    
    printf("\n========================================");
    printf("\n              РЕЗУЛЬТАТЫ");
    printf("\n========================================\n");
    printf("  Пройдено: %d\n", tests_passed);
    printf("  Провалено: %d\n", tests_failed);
    printf("  Всего: %d\n", tests_passed + tests_failed);
    printf("========================================\n");
    
    if (tests_failed == 0) {
        printf("  ✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n");
    } else {
        printf("  ✗ ЕСТЬ ОШИБКИ!\n");
    }
}

/* ============================================================================
 * Точка входа
 * ========================================================================== */
int main(void) {
    printf("========================================\n");
    printf("   ЛАБОРАТОРНАЯ РАБОТА №1, ВАРИАНТ 7");
    printf("\n   Полиморфная коллекция: СТРОКА");
    printf("\n========================================\n");
    
    run_all_tests();
    
    printf("\n=== НАЖМИТЕ ENTER ДЛЯ ВЫХОДА ===\n");
    getchar();
    
    return (tests_failed == 0) ? 0 : 1;
}