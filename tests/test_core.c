#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_macros.h"
#include "../src/dynamic_array.h"
#include "../src/fieldinfo.h"

// ═══════════════════════════════════════
// ТЕСТЫ: FieldInfo (мета-данные типа)
// ═══════════════════════════════════════

void test_fieldinfo_char(void) {
    TEST_GROUP("FieldInfo<char>: Singleton и функции");
    
    const FieldInfo* info1 = GetCharFieldInfo();
    const FieldInfo* info2 = GetCharFieldInfo();
    
    TEST(info1 != NULL, "GetCharFieldInfo возвращает не-NULL");
    TEST(info1 == info2, "Singleton: один экземпляр");
    TEST(info1->element_size == sizeof(char), "element_size == sizeof(char)");
    TEST(info1->copy != NULL, "copy функция определена");
    TEST(info1->destroy != NULL, "destroy функция определена");
    TEST(info1->compare != NULL, "compare функция определена");
    TEST(info1->print != NULL, "print функция определена");
    
    // Тест compare для char
    char a = 'a', b = 'b', c = 'a';
    TEST(info1->compare(&a, &b) < 0, "compare('a', 'b') < 0");
    TEST(info1->compare(&b, &a) > 0, "compare('b', 'a') > 0");
    TEST(info1->compare(&a, &c) == 0, "compare('a', 'a') == 0");
    
    TEST_GROUP_END();
}

void test_fieldinfo_int(void) {
    TEST_GROUP("FieldInfo<int>: Singleton и функции");
    
    const FieldInfo* info = GetIntFieldInfo();
    
    TEST(info != NULL, "GetIntFieldInfo возвращает не-NULL");
    TEST(info->element_size == sizeof(int), "element_size == sizeof(int)");
    TEST(info->copy != NULL, "copy функция определена");
    TEST(info->destroy != NULL, "destroy функция определена");
    
    // Тест compare для int
    int a = 10, b = 20, c = 10;
    TEST(info->compare(&a, &b) < 0, "compare(10, 20) < 0");
    TEST(info->compare(&b, &a) > 0, "compare(20, 10) > 0");
    TEST(info->compare(&a, &c) == 0, "compare(10, 10) == 0");
    
    TEST_GROUP_END();
}

// ═══════════════════════════════════════
// ТЕСТЫ: DynamicArray (полиморфный контейнер)
// ═══════════════════════════════════════

void test_dynamic_array_char(void) {
    TEST_GROUP("DynamicArray<char>: базовые операции");
    
    DynamicArray* arr = DynamicArray_Create(GetCharFieldInfo(), 4);
    TEST(arr != NULL, "DynamicArray_Create не-NULL");
    
    // Push
    char chars[] = {'H', 'e', 'l', 'l', 'o'};
    for (int i = 0; i < 5; i++) {
        int result = DynamicArray_Push(arr, &chars[i]);
        char msg[50];
        snprintf(msg, sizeof(msg), "Push('%c') == 0", chars[i]);
        TEST(result == 0, msg);
    }
    
    // Size
    TEST(DynamicArray_Size(arr) == 5, "Size == 5 после 5 Push");
    
    // Get
    for (int i = 0; i < 5; i++) {
        const char* c = (const char*)DynamicArray_Get(arr, i);
        char msg[50];
        snprintf(msg, sizeof(msg), "Get(%d) == '%c'", i, chars[i]);
        TEST(c != NULL && *c == chars[i], msg);
    }
    
    // Out of bounds
    TEST(DynamicArray_Get(arr, 10) == NULL, "Get(10) == NULL (out of bounds)");
    
    // Set
    char new_char = 'X';
    TEST(DynamicArray_Set(arr, 2, &new_char) == 0, "Set(2, 'X') == 0");
    const char* updated = (const char*)DynamicArray_Get(arr, 2);
    TEST(updated != NULL && *updated == 'X', "Get(2) после Set == 'X'");
    
    // CheckType
    TEST(DynamicArray_CheckType(arr, GetCharFieldInfo()) == 1, "CheckType(char) == 1");
    TEST(DynamicArray_CheckType(arr, GetIntFieldInfo()) == 0, "CheckType(int) == 0");
    
    DynamicArray_Destroy(arr);
    TEST_GROUP_END();
}

void test_dynamic_array_int(void) {
    TEST_GROUP("DynamicArray<int>: полиморфизм");
    
    DynamicArray* arr = DynamicArray_Create(GetIntFieldInfo(), 4);
    TEST(arr != NULL, "DynamicArray_Create<int> не-NULL");
    
    // Push int
    int nums[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        TEST(DynamicArray_Push(arr, &nums[i]) == 0, "Push(int) == 0");
    }
    
    // Get int
    for (int i = 0; i < 5; i++) {
        const int* val = (const int*)DynamicArray_Get(arr, i);
        char msg[50];
        snprintf(msg, sizeof(msg), "Get(%d) == %d", i, nums[i]);
        TEST(val != NULL && *val == nums[i], msg);
    }
    
    // Resize test
    for (int i = 0; i < 10; i++) {
        int x = 100 + i;
        DynamicArray_Push(arr, &x);
    }
    TEST(DynamicArray_Size(arr) == 15, "Size == 15 после 10 дополнительных Push");
    
    // Проверка данных после resize
    TEST(*(const int*)DynamicArray_Get(arr, 0) == 10, "Первый элемент не изменился");
    
    DynamicArray_Destroy(arr);
    TEST_GROUP_END();
}

void test_dynamic_array_edge_cases(void) {
    TEST_GROUP("DynamicArray: граничные случаи");
    
    // NULL параметры
    TEST(DynamicArray_Create(NULL, 10) == NULL, "Create(NULL) == NULL");
    TEST(DynamicArray_Push(NULL, NULL) == -1, "Push(NULL, ...) == -1");
    
    // Пустой массив
    DynamicArray* empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    TEST(DynamicArray_Size(empty) == 0, "Size пустого массива == 0");
    TEST(DynamicArray_Get(empty, 0) == NULL, "Get(0) пустого массива == NULL");
    DynamicArray_Destroy(empty);
    
    // Set out of bounds
    DynamicArray* arr = DynamicArray_Create(GetIntFieldInfo(), 4);
    int val = 42;
    DynamicArray_Push(arr, &val);
    TEST(DynamicArray_Set(arr, 10, &val) == -1, "Set(10, ...) == -1 (out of bounds)");
    DynamicArray_Destroy(arr);
    
    TEST_GROUP_END();
}

// ═══════════════════════════════════════
// Главный запуск тестов core
// ═══════════════════════════════════════

void run_core_tests(void) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   CORE TESTS — DynamicArray + FieldInfo║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    test_fieldinfo_char();
    test_fieldinfo_int();
    test_dynamic_array_char();
    test_dynamic_array_int();
    test_dynamic_array_edge_cases();
}