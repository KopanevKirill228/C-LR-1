// tests/test_core.c
#include "test_macros.h"
#include "Vector.h"
#include "fieldinfo.h"
#include <string.h>

/* ============================================================================
 * Тесты: FieldInfo (char_type.c)
 * ========================================================================== */
void test_fieldinfo_basics(void) {
    TEST_GROUP("FieldInfo: Singleton и функции");
    
    // Ленивая инициализация
    const FieldInfo* info1 = GetCharFieldInfo();
    const FieldInfo* info2 = GetCharFieldInfo();
    TEST_RUN("GetCharFieldInfo возвращает не-NULL", info1 != NULL);
    TEST_RUN("Singleton: один экземпляр", info1 == info2);
    
    // Проверка полей
    TEST_RUN("element_size == sizeof(char)", info1->element_size == sizeof(char));
    TEST_RUN("copy функция определена", info1->copy != NULL);
    TEST_RUN("destroy функция определена", info1->destroy != NULL);
    TEST_RUN("compare функция определена", info1->compare != NULL);
    
    // Тест char_compare
    char a = 'A', b = 'B', c = 'A';
    TEST_RUN("compare('A','B') < 0", info1->compare(&a, &b) < 0);
    TEST_RUN("compare('B','A') > 0", info1->compare(&b, &a) > 0);
    TEST_RUN("compare('A','A') == 0", info1->compare(&a, &c) == 0);
    
    // Граничный случай: NULL в compare
    TEST_RUN("compare(NULL, NULL) == 0", info1->compare(NULL, NULL) == 0);
    TEST_RUN("compare(NULL, &a) < 0", info1->compare(NULL, &a) < 0);
    
    TEST_GROUP_END();
}

/* ============================================================================
 * Тесты: Vector (базовые операции)
 * ========================================================================== */
void test_vector_basics(void) {
    TEST_GROUP("Vector: Create, Push, Get, Size");
    
    const FieldInfo* info = GetCharFieldInfo();
    
    // Создание
    Vector* vec = Vector_Create(info, 4);
    TEST_RUN("Vector_Create != NULL", vec != NULL);
    TEST_RUN("Начальный size == 0", Vector_Size(vec) == 0);
    
    // Push элементов
    char x = 'X', y = 'Y', z = 'Z';
    TEST_RUN("Push('X') == 0", Vector_Push(vec, &x) == 0);
    TEST_RUN("Push('Y') == 0", Vector_Push(vec, &y) == 0);
    TEST_RUN("Push('Z') == 0", Vector_Push(vec, &z) == 0);
    TEST_RUN("Size после 3 Push == 3", Vector_Size(vec) == 3);
    
    // Get элементов
    const char* g0 = (const char*)Vector_Get(vec, 0);
    const char* g1 = (const char*)Vector_Get(vec, 1);
    const char* g2 = (const char*)Vector_Get(vec, 2);
    TEST_RUN("Get(0) == 'X'", g0 != NULL && *g0 == 'X');
    TEST_RUN("Get(1) == 'Y'", g1 != NULL && *g1 == 'Y');
    TEST_RUN("Get(2) == 'Z'", g2 != NULL && *g2 == 'Z');
    
    // Граничные случаи Get
    TEST_RUN("Get(invalid index) == NULL", Vector_Get(vec, 10) == NULL);
    TEST_RUN("Get(NULL vec) == NULL", Vector_Get(NULL, 0) == NULL);
    
    // Set элемента
    char new_val = 'W';
    TEST_RUN("Set(1, 'W') == 0", Vector_Set(vec, 1, &new_val) == 0);
    const char* g1_after = (const char*)Vector_Get(vec, 1);
    TEST_RUN("Get(1) после Set == 'W'", g1_after != NULL && *g1_after == 'W');
    
    // Граничные случаи Set
    TEST_RUN("Set(invalid index) == -1", Vector_Set(vec, 10, &new_val) == -1);
    TEST_RUN("Set(NULL vec) == -1", Vector_Set(NULL, 0, &new_val) == -1);
    
    // CheckType
    TEST_RUN("CheckType с правильным типом == 1", Vector_CheckType(vec, info) == 1);
    TEST_RUN("CheckType с NULL == 0", Vector_CheckType(vec, NULL) == 0);
    
    // Destroy (без краша)
    Vector_Destroy(vec);
    TEST_RUN("Vector_Destroy безопасен", true);
    Vector_Destroy(NULL);  // Двойной destroy
    TEST_RUN("Vector_Destroy(NULL) безопасен", true);
    
    TEST_GROUP_END();
}

/* ============================================================================
 * Тесты: Vector (расширенные: resize, empty, NULL)
 * ========================================================================== */
void test_vector_advanced(void) {
    TEST_GROUP("Vector: Resize, Empty, NULL handling");
    
    const FieldInfo* info = GetCharFieldInfo();
    
    // Пустой вектор
    Vector* empty = Vector_Create(info, 0);
    TEST_RUN("Create с capacity=0 работает", empty != NULL);
    TEST_RUN("Empty vector size == 0", Vector_Size(empty) == 0);
    Vector_Destroy(empty);
    
    // Автоматический resize
    Vector* vec = Vector_Create(info, 2);
    for (int i = 0; i < 10; i++) {
        char c = 'A' + i;
        Vector_Push(vec, &c);
    }
    TEST_RUN("Push 10 элементов в vec(2) работает", Vector_Size(vec) == 10);
    
    // Проверка данных после resize
    const char* first = (const char*)Vector_Get(vec, 0);
    const char* last = (const char*)Vector_Get(vec, 9);
    TEST_RUN("First element после resize == 'A'", first && *first == 'A');
    TEST_RUN("Last element после resize == 'J'", last && *last == 'J');
    
    Vector_Destroy(vec);
    
    // NULL параметры
    TEST_RUN("Create(NULL type) == NULL", Vector_Create(NULL, 4) == NULL);
    TEST_RUN("Push(NULL vec) == -1", Vector_Push(NULL, NULL) == -1);
    
    TEST_GROUP_END();
}