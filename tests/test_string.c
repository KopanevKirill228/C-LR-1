// tests/test_string.c
#include "test_macros.h"  // ← Подключаем макросы
#include "my_string.h"
#include "fieldinfo.h"

void test_string_create(void) {
    String* s = String_Create("Hello");
    TEST_ASSERT(s != NULL, "String_Create != NULL");
    TEST_ASSERT_EQ(5, String_Length(s), "Длина == 5");
    String_Destroy(s);

    String* empty = String_Create("");
    TEST_ASSERT(empty != NULL, "Пустая строка != NULL");
    TEST_ASSERT_EQ(0, String_Length(empty), "Длина пустой == 0");
    String_Destroy(empty);

    String* null_str = String_Create(NULL);
    TEST_ASSERT(null_str != NULL, "String_Create(NULL) != NULL");
    String_Destroy(null_str);
}

void test_string_concat(void) {
    String* s1 = String_Create("Hello, ");
    String* s2 = String_Create("World!");
    String* result = String_Concat(s1, s2);
    TEST_ASSERT(result != NULL, "Concat != NULL");
    TEST_ASSERT_EQ(13, String_Length(result), "Длина конкатенации == 13");
    String_Destroy(s1);
    String_Destroy(s2);
    String_Destroy(result);

    String* null_result = String_Concat(NULL, s2);
    TEST_ASSERT(null_result == NULL, "Concat с NULL возвращает NULL");
}

void test_string_substring(void) {
    String* text = String_Create("Hello, World!");
    String* sub = String_Substring(text, 0, 5);
    TEST_ASSERT(sub != NULL, "Substring != NULL");
    TEST_ASSERT_EQ(5, String_Length(sub), "Длина подстроки == 5");
    String_Destroy(text);
    String_Destroy(sub);

    String* err = String_Substring(text, 10, 5);
    TEST_ASSERT(err == NULL, "Substring start > end возвращает NULL");
}

void test_string_find(void) {
    String* text = String_Create("Hello, hello, HELLO!");
    String* pattern = String_Create("hello");
    size_t count = 0;
    int* indices = String_Find(text, pattern, false, &count);
    TEST_ASSERT(indices != NULL, "Find нашёл совпадения");
    TEST_ASSERT(count >= 1, "Найдено хотя бы 1 совпадение");
    if (indices) free(indices);
    String_Destroy(text);
    String_Destroy(pattern);
}