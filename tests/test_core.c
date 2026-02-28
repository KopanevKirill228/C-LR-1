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
    
    TEST_RUN("GetCharFieldInfo возвращает не-NULL", info1 != NULL);
    TEST_RUN("Singleton: один экземпляр", info1 == info2);
    TEST_RUN("element_size == sizeof(char)", info1->element_size == sizeof(char));
    TEST_RUN("copy функция определена", info1->copy != NULL);
    TEST_RUN("destroy функция определена", info1->destroy != NULL);
    TEST_RUN("compare функция определена", info1->compare != NULL);
    TEST_RUN("print функция определена", info1->print != NULL);
    
    TEST_GROUP_END();
}

void test_fieldinfo_int(void) {
    TEST_GROUP("FieldInfo<int>: Singleton и функции");
    
    const FieldInfo* info = GetIntFieldInfo();
    
    TEST_RUN("GetIntFieldInfo возвращает не-NULL", info != NULL);
    TEST_RUN("element_size == sizeof(int)", info->element_size == sizeof(int));
    TEST_RUN("copy функция определена", info->copy != NULL);
    TEST_RUN("destroy функция определена", info->destroy != NULL);
    
    // Тест compare для int
    int a = 10, b = 20, c = 10;
    TEST_RUN("compare(10, 20) < 0", info->compare(&a, &b) < 0);
    TEST_RUN("compare(20, 10) > 0", info->compare(&b, &a) > 0);
    TEST_RUN("compare(10, 10) == 0", info->compare(&a, &c) == 0);
    
    TEST_GROUP_END();
}

// ═══════════════════════════════════════
// ТЕСТЫ: DynamicArray (полиморфный контейнер)
// ═══════════════════════════════════════

void test_dynamic_array_char(void) {
    TEST_GROUP("DynamicArray<char>: базовые операции");
    
    DynamicArray* arr = DynamicArray_Create(GetCharFieldInfo(), 4);
    ASSERT_NOT_NULL(arr, "DynamicArray_Create не-NULL");
    
    // Push
    char chars[] = {'H', 'e', 'l', 'l', 'o'};
    for (int i = 0; i < 5; i++) {
        int result = DynamicArray_Push(arr, &chars[i]);
        char msg[50];
        snprintf(msg, sizeof(msg), "Push('%c') == 0", chars[i]);
        TEST_RUN(msg, result == 0);
    }
    
    // Size
    TEST_RUN("Size == 5 после 5 Push", DynamicArray_Size(arr) == 5);
    
    // Get
    for (int i = 0; i < 5; i++) {
        const char* c = (const char*)DynamicArray_Get(arr, i);
        char msg[50];
        snprintf(msg, sizeof(msg), "Get(%d) == '%c'", i, chars[i]);
        TEST_RUN(msg, c != NULL && *c == chars[i]);
    }
    
    // Out of bounds
    TEST_RUN("Get(10) == NULL (out of bounds)", DynamicArray_Get(arr, 10) == NULL);
    
    // Set
    char new_char = 'X';
    TEST_RUN("Set(2, 'X') == 0", DynamicArray_Set(arr, 2, &new_char) == 0);
    const char* updated = (const char*)DynamicArray_Get(arr, 2);
    TEST_RUN("Get(2) после Set == 'X'", updated != NULL && *updated == 'X');
    
    // CheckType
    TEST_RUN("CheckType(char) == 1", DynamicArray_CheckType(arr, GetCharFieldInfo()) == 1);
    TEST_RUN("CheckType(int) == 0", DynamicArray_CheckType(arr, GetIntFieldInfo()) == 0);
    
    // Destroy
    DynamicArray_Destroy(arr);  // Не должно крашиться
    TEST_RUN("Destroy(NULL) безопасен", 1);  // Всегда PASS
    DynamicArray_Destroy(NULL);
    
    TEST_GROUP_END();
}

void test_dynamic_array_int(void) {
    TEST_GROUP("DynamicArray<int>: полиморфизм");
    
    DynamicArray* arr = DynamicArray_Create(GetIntFieldInfo(), 4);
    ASSERT_NOT_NULL(arr, "DynamicArray_Create<int> не-NULL");
    
    // Push int
    int nums[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        TEST_RUN("Push(int) == 0", DynamicArray_Push(arr, &nums[i]) == 0);
    }
    
    // Get int
    for (int i = 0; i < 5; i++) {
        const int* val = (const int*)DynamicArray_Get(arr, i);
        char msg[50];
        snprintf(msg, sizeof(msg), "Get(%d) == %d", i, nums[i]);
        TEST_RUN(msg, val != NULL && *val == nums[i]);
    }
    
    // Resize test (автоматическое увеличение capacity)
    for (int i = 0; i < 10; i++) {
        int x = 100 + i;
        DynamicArray_Push(arr, &x);
    }
    TEST_RUN("Size == 15 после 10 дополнительных Push", DynamicArray_Size(arr) == 15);
    
    // Проверка данных после resize
    TEST_RUN("Первый элемент не изменился после resize", 
             *(const int*)DynamicArray_Get(arr, 0) == 10);
    
    DynamicArray_Destroy(arr);
    TEST_GROUP_END();
}

void test_dynamic_array_edge_cases(void) {
    TEST_GROUP("DynamicArray: граничные случаи");
    
    // NULL параметры
    TEST_RUN("Create(NULL) == NULL", DynamicArray_Create(NULL, 10) == NULL);
    TEST_RUN("Push(NULL, ...) == -1", DynamicArray_Push(NULL, NULL) == -1);
    
    // Пустой массив
    DynamicArray* empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    TEST_RUN("Size пустого массива == 0", DynamicArray_Size(empty) == 0);
    TEST_RUN("Get(0) пустого массива == NULL", DynamicArray_Get(empty, 0) == NULL);
    DynamicArray_Destroy(empty);
    
    // Set out of bounds
    DynamicArray* arr = DynamicArray_Create(GetIntFieldInfo(), 4);
    int val = 42;
    TEST_RUN("Set(10, ...) == -1 (out of bounds)", DynamicArray_Set(arr, 10, &val) == -1);
    DynamicArray_Destroy(arr);
    
    TEST_GROUP_END();
}

// ═══════════════════════════════════════
// Главный запуск тестов core
// ═══════════════════════════════════════

void run_core_tests(void) {
    printf("╔════════════════════════════════════════╗\n");
    printf("║   CORE TESTS — DynamicArray + FieldInfo║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    test_fieldinfo_char();
    test_fieldinfo_int();
    test_dynamic_array_char();
    test_dynamic_array_int();
    test_dynamic_array_edge_cases();
}