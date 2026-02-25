// tests/test_fieldinfo.c
#include "test_macros.h"  // ← Подключаем макросы
#include "fieldinfo.h"

void test_fieldinfo(void) {
    const FieldInfo* info1 = GetCharFieldInfo();
    const FieldInfo* info2 = GetCharFieldInfo();
    TEST_ASSERT(info1 != NULL, "GetCharFieldInfo != NULL");
    TEST_ASSERT(info1 == info2, "Singleton работает");
    TEST_ASSERT(info1->element_size == sizeof(char), "element_size == 1");
}