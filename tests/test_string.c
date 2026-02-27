#include "test_macros.h"
#include "my_string.h"
#include "fieldinfo.h"
#include <string.h>

// Вспомогательная функция для красивого вывода строки
static const char* str_or_null(const String* s) {
    if (s == NULL) return "NULL";
    const char* cstr = String_ToCString(s);
    return cstr ? cstr : "(empty)";
}

// Тесты: String_Create, String_Destroy
void test_string_create_destroy(void) {
    TEST_GROUP("String: Create / Destroy");
    
    // Нормальное создание
    String* s1 = String_Create("Hello");
    TEST_SHOW("Create(\"Hello\")", 
              s1 != NULL && String_Length(s1) == 5,
              "\"Hello\"", 
              "String != NULL, length=5",
              str_or_null(s1));
    String_Destroy(s1);
    
    // Пустая строка
    String* empty = String_Create("");
    TEST_SHOW("Create(\"\")", 
              empty != NULL && String_Length(empty) == 0,
              "\"\"", 
              "Empty string, length=0",
              str_or_null(empty));
    String_Destroy(empty);
    
    // NULL вход
    String* null_str = String_Create(NULL);
    TEST_SHOW("Create(NULL)", 
              null_str != NULL && String_Length(null_str) == 0,
              "NULL", 
              "Empty string (safe)",
              str_or_null(null_str));
    String_Destroy(null_str);
    
    // Destroy безопасность
    String* s2 = String_Create("Test");
    String_Destroy(s2);
    TEST_RUN("Destroy не крашит", true);
    String_Destroy(s2);  // Двойной
    TEST_RUN("Double Destroy безопасен", true);
    String_Destroy(NULL);
    TEST_RUN("Destroy(NULL) безопасен", true);
    
    TEST_GROUP_END();
}


// Тесты: String_Concat

void test_string_concat(void) {
    TEST_GROUP("String: Concatenation");
    
    String* s1 = String_Create("Hello, ");
    String* s2 = String_Create("World!");
    
    // Нормальная конкатенация
    String* result = String_Concat(s1, s2);
    TEST_SHOW("Concat(\"Hello, \", \"World!\")", 
              result != NULL && strcmp(String_ToCString(result), "Hello, World!") == 0,
              "\"Hello, \" + \"World!\"", 
              "\"Hello, World!\"",
              str_or_null(result));
    String_Destroy(result);
    
    // Конкатенация с пустой
    String* empty = String_Create("");
    String* r1 = String_Concat(empty, s2);
    TEST_SHOW("Concat(\"\", \"World!\")", 
              r1 != NULL && strcmp(String_ToCString(r1), "World!") == 0,
              "\"\" + \"World!\"", 
              "\"World!\"",
              str_or_null(r1));
    String_Destroy(r1);
    
    String* r2 = String_Concat(s1, empty);
    TEST_SHOW("Concat(\"Hello, \", \"\")", 
              r2 != NULL && strcmp(String_ToCString(r2), "Hello, ") == 0,
              "\"Hello, \" + \"\"", 
              "\"Hello, \"",
              str_or_null(r2));
    String_Destroy(r2);
    String_Destroy(empty);
    
    // Граничные: NULL
    String* r_null1 = String_Concat(NULL, s2);
    TEST_SHOW("Concat(NULL, \"World!\")", 
              r_null1 == NULL,
              "NULL + \"World!\"", 
              "NULL (safe)",
              r_null1 == NULL ? "NULL" : str_or_null(r_null1));
    
    String* r_null2 = String_Concat(s1, NULL);
    TEST_SHOW("Concat(\"Hello, \", NULL)", 
              r_null2 == NULL,
              "\"Hello, \" + NULL", 
              "NULL (safe)",
              r_null2 == NULL ? "NULL" : str_or_null(r_null2));
    
    String_Destroy(s1);
    String_Destroy(s2);
    
    TEST_GROUP_END();
}


//Тесты: String_Substring

void test_string_substring(void) {
    TEST_GROUP("String: Substring");
    
    String* text = String_Create("Hello, World!");
    
    // Нормальная подстрока
    String* sub1 = String_Substring(text, 0, 5);
    TEST_SHOW("Substring[0,5) of \"Hello, World!\"", 
              sub1 != NULL && strcmp(String_ToCString(sub1), "Hello") == 0,
              "[0,5)", 
              "\"Hello\"",
              str_or_null(sub1));
    String_Destroy(sub1);
    
    // Подстрока из середины
    String* sub2 = String_Substring(text, 7, 12);
    TEST_SHOW("Substring[7,12) of \"Hello, World!\"", 
              sub2 != NULL && strcmp(String_ToCString(sub2), "World") == 0,
              "[7,12)", 
              "\"World\"",
              str_or_null(sub2));
    String_Destroy(sub2);
    
    // Пустая подстрока
    String* sub_empty = String_Substring(text, 5, 5);
    TEST_SHOW("Substring[5,5) (empty range)", 
              sub_empty != NULL && String_Length(sub_empty) == 0,
              "[5,5)", 
              "\"\" (empty)",
              str_or_null(sub_empty));
    String_Destroy(sub_empty);
    
    // Граничные случаи: ошибки
    String* err1 = String_Substring(text, 10, 5);
    TEST_SHOW("Substring[10,5) (start > end)", 
              err1 == NULL,
              "[10,5)", 
              "NULL (invalid)",
              err1 == NULL ? "NULL" : str_or_null(err1));
    
    String* err2 = String_Substring(text, 0, 100);
    TEST_SHOW("Substring[0,100) (end > length)", 
              err2 == NULL,
              "[0,100)", 
              "NULL (invalid)",
              err2 == NULL ? "NULL" : str_or_null(err2));
    
    String* err3 = String_Substring(NULL, 0, 5);
    TEST_SHOW("Substring from NULL string", 
              err3 == NULL,
              "NULL input", 
              "NULL (safe)",
              err3 == NULL ? "NULL" : str_or_null(err3));
    
    String_Destroy(text);
    
    TEST_GROUP_END();
}


//Тесты: String_Find

void test_string_find(void) {
    TEST_GROUP("String: Find (search)");
    
    String* text = String_Create("Hello, hello, HELLO!");
    String* pattern = String_Create("hello");
    
    // Case-sensitive поиск
    size_t count_cs = 0;
    int* idx_cs = String_Find(text, pattern, true, &count_cs);
    TEST_SHOW("Find(\"hello\", case-sensitive)", 
              idx_cs != NULL && count_cs == 1,
              "text=\"Hello, hello, HELLO!\", pattern=\"hello\", sensitive=true", 
              "count=1, pos=[7]",
              idx_cs ? "found" : "NULL");
    if (idx_cs) {
        printf("│        Positions: [%d]\n", idx_cs[0]);
        free(idx_cs);
    }
    
    // Case-insensitive поиск
    size_t count_ci = 0;
    int* idx_ci = String_Find(text, pattern, false, &count_ci);
    TEST_SHOW("Find(\"hello\", case-insensitive)", 
              idx_ci != NULL && count_ci == 3,
              "text=\"Hello, hello, HELLO!\", pattern=\"hello\", sensitive=false", 
              "count=3, pos=[0,7,14]",
              idx_ci ? "found" : "NULL");
    if (idx_ci) {
        printf("│        Positions: [%d, %d, %d]\n", idx_ci[0], idx_ci[1], idx_ci[2]);
        free(idx_ci);
    }
    
    // Поиск несуществующего
    String* not_found = String_Create("xyz");
    size_t count_nf = 0;
    int* idx_nf = String_Find(text, not_found, true, &count_nf);
    TEST_SHOW("Find(\"xyz\", not exists)", 
              idx_nf == NULL && count_nf == 0,
              "pattern=\"xyz\"", 
              "NULL, count=0",
              idx_nf == NULL ? "NULL" : "found");
    String_Destroy(not_found);
    
    // Граничные: пустой паттерн
    String* empty_pat = String_Create("");
    size_t count_ep = 0;
    int* idx_ep = String_Find(text, empty_pat, true, &count_ep);
    TEST_SHOW("Find(\"\") (empty pattern)", 
              idx_ep == NULL,
              "empty pattern", 
              "NULL (invalid)",
              idx_ep == NULL ? "NULL" : "found");
    String_Destroy(empty_pat);
    
    // Граничные: NULL параметры
    size_t dummy = 0;
    int* idx_null1 = String_Find(NULL, pattern, true, &dummy);
    TEST_SHOW("Find in NULL text", 
              idx_null1 == NULL,
              "text=NULL", 
              "NULL (safe)",
              idx_null1 == NULL ? "NULL" : "found");
    
    int* idx_null2 = String_Find(text, NULL, true, &dummy);
    TEST_SHOW("Find with NULL pattern", 
              idx_null2 == NULL,
              "pattern=NULL", 
              "NULL (safe)",
              idx_null2 == NULL ? "NULL" : "found");
    
    String_Destroy(text);
    String_Destroy(pattern);
    
    TEST_GROUP_END();
}


//Тесты: String_Utils (Length, ToCString, cache)

void test_string_utils(void) {
    TEST_GROUP("String: Utilities (Length, ToCString, cache)");
    
    // Length
    String* s1 = String_Create("Test");
    TEST_RUN("Length(\"Test\") == 4", String_Length(s1) == 4);
    String_Destroy(s1);
    
    String* empty = String_Create("");
    TEST_RUN("Length(\"\") == 0", String_Length(empty) == 0);
    String_Destroy(empty);
    
    TEST_RUN("Length(NULL) == 0", String_Length(NULL) == 0);
    
    // ToCString и кэширование
    String* s2 = String_Create("Cache");
    const char* cstr1 = String_ToCString(s2);
    const char* cstr2 = String_ToCString(s2);
    TEST_SHOW("ToCString cache (same pointer)", 
              cstr1 == cstr2 && cstr1 != NULL,
              "Two calls to ToCString", 
              "Same pointer returned",
              cstr1 == cstr2 ? "YES" : "NO");
    TEST_RUN("ToCString content correct", strcmp(cstr1, "Cache") == 0);
    String_Destroy(s2);
    
    // ToCString для NULL
    TEST_RUN("ToCString(NULL) == NULL", String_ToCString(NULL) == NULL);
    
    TEST_GROUP_END();
}