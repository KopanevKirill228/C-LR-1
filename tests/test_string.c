#include "test_macros.h"
#include "../src/my_string.h"
#include "../src/dynamic_array.h"
#include "../src/fieldinfo.h"

// Вспомогательная функция: создаёт DynamicArray с данными
static DynamicArray* create_da_from_chars(const char* str) {
    DynamicArray* da = DynamicArray_Create(GetCharFieldInfo(), strlen(str));
    for(size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        DynamicArray_Push(da, &c);
    }
    return da;
}

// Вспомогательная функция: создаёт String из DynamicArray
static String* create_string_from_da(DynamicArray* da) {
    size_t size = DynamicArray_Size(da);
    if(size == 0) return String_CreateEmpty(4, GetCharFieldInfo());
    
    // Копируем данные из DynamicArray в буфер
    unsigned char* buffer = malloc(size * sizeof(char));
    for(size_t i = 0; i < size; i++) {
        const char* c = (const char*)DynamicArray_Get(da, i);
        buffer[i] = *c;
    }
    
    String* s = String_Create(buffer, size, GetCharFieldInfo());
    free(buffer);
    return s;
}

void test_string_create_edge_cases(void) {
    printf("\n=== String Create Edge Cases ===\n");
    
    // Граничный случай 1: NULL type_info
    String* s = String_Create(NULL, 0, NULL);
    TEST(s == NULL, "Create with NULL type_info returns NULL");
    
    // Граничный случай 2: Пустой DynamicArray
    DynamicArray* empty_da = DynamicArray_Create(GetCharFieldInfo(), 0);
    s = create_string_from_da(empty_da);
    TEST(s != NULL, "Create from empty DA works");
    TEST(String_Length(s) == 0, "Empty string length = 0");
    String_Destroy(s);
    DynamicArray_Destroy(empty_da);
    
    // Граничный случай 3: Создание с нулевой capacity
    s = String_CreateEmpty(0, GetCharFieldInfo());
    TEST(s != NULL, "CreateEmpty with 0 capacity works");
    TEST(String_Length(s) == 0, "Length = 0");
    String_Destroy(s);
    
    // Граничный случай 4: Огромная capacity
    s = String_CreateEmpty(1000000, GetCharFieldInfo());
    TEST(s != NULL, "CreateEmpty with huge capacity works");
    String_Destroy(s);
}

void test_string_concat_edge_cases(void) {
    printf("\n=== Concat Edge Cases ===\n");
    
    // Подготовка данных через DynamicArray
    DynamicArray* da1 = create_da_from_chars("Hello");
    DynamicArray* da2 = create_da_from_chars(" World");
    DynamicArray* da_empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    
    String* s1 = create_string_from_da(da1);
    String* s2 = create_string_from_da(da2);
    String* s_empty = create_string_from_da(da_empty);
    
    // Граничный случай 1: Конкатенация с пустой строкой
    String* result = String_Concat(s1, s_empty);
    TEST(result != NULL, "Concat with empty works");
    TEST(String_Length(result) == 5, "Length = 5 (same as s1)");
    
    // Проверка содержимого
    int correct = 1;
    for(int i = 0; i < 5; i++) {
        const char* c1 = (const char*)String_GetElement(s1, i);
        const char* c2 = (const char*)String_GetElement(result, i);
        if(c1 == NULL || c2 == NULL || *c1 != *c2) {
            correct = 0;
            break;
        }
    }
    TEST(correct, "Result equals original when concat with empty");
    String_Destroy(result);
    
    // Граничный случай 2: Конкатенация с NULL
    TEST(String_Concat(s1, NULL) == NULL, "Concat with NULL returns NULL");
    TEST(String_Concat(NULL, s1) == NULL, "Concat with NULL returns NULL");
    
    // Граничный случай 3: Конкатенация двух пустых
    result = String_Concat(s_empty, s_empty);
    TEST(result != NULL, "Concat two empty strings");
    TEST(String_Length(result) == 0, "Result empty");
    String_Destroy(result);
    
    // Граничный случай 4: Очень длинная конкатенация
    DynamicArray* da_long = DynamicArray_Create(GetCharFieldInfo(), 0);
    for(int i = 0; i < 1000; i++) {
        char c = 'a';
        DynamicArray_Push(da_long, &c);
    }
    String* s_long = create_string_from_da(da_long);
    
    result = String_Concat(s_long, s_long);
    TEST(result != NULL, "Concat two long strings");
    TEST(String_Length(result) == 2000, "Length = 2000");
    String_Destroy(result);
    
    String_Destroy(s_long);
    DynamicArray_Destroy(da_long);
    
    String_Destroy(s1);
    String_Destroy(s2);
    String_Destroy(s_empty);
    DynamicArray_Destroy(da1);
    DynamicArray_Destroy(da2);
    DynamicArray_Destroy(da_empty);
}

void test_string_substring_edge_cases(void) {
    printf("\n=== Substring Edge Cases ===\n");
    
    DynamicArray* da = create_da_from_chars("Hello World");
    String* s = create_string_from_da(da);
    size_t len = String_Length(s);
    
    // Граничный случай 1: Пустая подстрока (start == end)
    String* sub = String_Substring(s, 5, 5);
    TEST(sub != NULL, "Empty substring (start == end)");
    TEST(String_Length(sub) == 0, "Empty substring length = 0");
    String_Destroy(sub);
    
    // Граничный случай 2: Подстрока из одного символа
    sub = String_Substring(s, 0, 1);
    TEST(sub != NULL, "Single char substring [0,1)");
    TEST(String_Length(sub) == 1, "Length = 1");
    const char* c = (const char*)String_GetElement(sub, 0);
    TEST(c != NULL && *c == 'H', "Single char is 'H'");
    String_Destroy(sub);
    
    // Граничный случай 3: Подстрока до конца
    sub = String_Substring(s, 6, len);
    TEST(sub != NULL, "Substring [6,end)");
    TEST(String_Length(sub) == 5, "Length = 5");
    
    // Проверка "World"
    const char* world = "World";
    int correct = 1;
    for(int i = 0; i < 5; i++) {
        c = (const char*)String_GetElement(sub, i);
        if(c == NULL || *c != world[i]) {
            correct = 0;
            break;
        }
    }
    TEST(correct, "Substring [6,end) = 'World'");
    String_Destroy(sub);
    
    // Граничный случай 4: Подстрока с start = 0, end = 0
    sub = String_Substring(s, 0, 0);
    TEST(sub != NULL, "Empty substring at start");
    TEST(String_Length(sub) == 0, "Length = 0");
    String_Destroy(sub);
    
    // Граничный случай 5: Неверные индексы
    TEST(String_Substring(s, 5, 3) == NULL, "start > end returns NULL");
    TEST(String_Substring(s, len, len+1) == NULL, "end > length returns NULL");
    TEST(String_Substring(s, len+1, len+2) == NULL, "start > length returns NULL");
    TEST(String_Substring(s, -1, 5) == NULL, "negative start (wrapped) returns NULL");
    
    // Граничный случай 6: Substring от NULL
    TEST(String_Substring(NULL, 0, 5) == NULL, "Substring from NULL returns NULL");
    
    String_Destroy(s);
    DynamicArray_Destroy(da);
}

void test_string_find_edge_cases(void) {
    printf("\n=== Find Edge Cases ===\n");
    
    // Создаём строку "abababa"
    DynamicArray* da_text = DynamicArray_Create(GetCharFieldInfo(), 7);
    char chars[] = {'a','b','a','b','a','b','a'};
    for(int i = 0; i < 7; i++) {
        DynamicArray_Push(da_text, &chars[i]);
    }
    String* text = create_string_from_da(da_text);
    
    // Граничный случай 1: Пустой паттерн
    DynamicArray* da_empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    String* empty_pat = create_string_from_da(da_empty);
    
    size_t count;
    int* indices = String_Find(text, empty_pat, true, &count);
    TEST(indices == NULL && count == 0, "Empty pattern not found");
    
    // Граничный случай 2: Паттерн длиннее текста
    DynamicArray* da_long = create_da_from_chars("abababaabababa");
    String* long_pat = create_string_from_da(da_long);
    
    indices = String_Find(text, long_pat, true, &count);
    TEST(indices == NULL && count == 0, "Pattern longer than text not found");
    String_Destroy(long_pat);
    DynamicArray_Destroy(da_long);
    
    // Граничный случай 3: Паттерн в самом начале
    DynamicArray* da_start = create_da_from_chars("aba");
    String* start_pat = create_string_from_da(da_start);
    
    indices = String_Find(text, start_pat, true, &count);
    TEST(count == 3, "Pattern 'aba' appears 3 times");
    if(indices) {
        TEST(indices[0] == 0, "First at position 0");
        TEST(indices[1] == 2, "Second at position 2");
        TEST(indices[2] == 4, "Third at position 4");
        free(indices);
    }
    String_Destroy(start_pat);
    DynamicArray_Destroy(da_start);
    
    // Граничный случай 4: Паттерн в самом конце
    DynamicArray* da_end = create_da_from_chars("aba");
    String* end_pat = create_string_from_da(da_end);
    
    indices = String_Find(text, end_pat, true, &count);
    TEST(count == 3, "Pattern at end also appears 3 times");
    if(indices) free(indices);
    String_Destroy(end_pat);
    DynamicArray_Destroy(da_end);
    
    // Граничный случай 5: Паттерн из одного символа
    DynamicArray* da_one = create_da_from_chars("a");
    String* one_pat = create_string_from_da(da_one);
    
    indices = String_Find(text, one_pat, true, &count);
    TEST(count == 4, "Single char 'a' appears 4 times");
    if(indices) free(indices);
    String_Destroy(one_pat);
    DynamicArray_Destroy(da_one);
    
    // Граничный случай 6: Поиск с регистром
    DynamicArray* da_mixed = create_da_from_chars("AbA");
    String* mixed_pat = create_string_from_da(da_mixed);
    
    indices = String_Find(text, mixed_pat, true, &count);
    TEST(count == 0 && indices == NULL, "Case sensitive: different case not found");
    
    String_Destroy(mixed_pat);
    DynamicArray_Destroy(da_mixed);
    
    // Граничный случай 7: Поиск с NULL параметрами
    TEST(String_Find(NULL, text, true, &count) == NULL, "Find with NULL text");
    TEST(String_Find(text, NULL, true, &count) == NULL, "Find with NULL pattern");
    TEST(String_Find(text, text, true, NULL) == NULL, "Find with NULL out_count");
    
    // Граничный случай 8: Поиск разных типов
    DynamicArray* da_int = DynamicArray_Create(GetIntFieldInfo(), 1);
    int num = 42;
    DynamicArray_Push(da_int, &num);
    String* int_str = create_string_from_da(da_int);  // Это всё равно char строка!
    // Правильно создаём int строку отдельно
    String* int_string = String_Create(&num, 1, GetIntFieldInfo());
    
    indices = String_Find(text, int_string, true, &count);
    TEST(indices == NULL && count == 0, "Different types not found");
    
    String_Destroy(int_string);
    DynamicArray_Destroy(da_int);
    
    String_Destroy(text);
    String_Destroy(empty_pat);
    DynamicArray_Destroy(da_text);
    DynamicArray_Destroy(da_empty);
}

void test_string_type_mismatch(void) {
    printf("\n=== Type Mismatch Tests ===\n");
    
    // Создаём char строку через DynamicArray
    DynamicArray* da_char = create_da_from_chars("Hello");
    String* s_char = create_string_from_da(da_char);
    
    // Создаём int строку через DynamicArray (но для int нужен другой подход)
    DynamicArray* da_int = DynamicArray_Create(GetIntFieldInfo(), 3);
    int nums[] = {1, 2, 3};
    for(int i = 0; i < 3; i++) {
        DynamicArray_Push(da_int, &nums[i]);
    }
    
    // Для int строки используем String_Create напрямую с int данными
    String* s_int = String_Create(nums, 3, GetIntFieldInfo());
    
    // Граничный случай 1: Проверка разных типов
    TEST(String_SameType(s_char, s_int) == false, "Char and Int are different types");
    
    // Граничный случай 2: Конкатенация разных типов
    String* result = String_Concat(s_char, s_int);
    TEST(result == NULL, "Concat different types fails");
    
    // Граничный случай 3: Поиск с разными типами
    size_t count;
    int* indices = String_Find(s_char, s_int, true, &count);
    TEST(indices == NULL && count == 0, "Find with different types fails");
    
    String_Destroy(s_char);
    String_Destroy(s_int);
    DynamicArray_Destroy(da_char);
    DynamicArray_Destroy(da_int);
}

void run_string_tests(void) {
    test_string_create_edge_cases();
    test_string_concat_edge_cases();
    test_string_substring_edge_cases();
    test_string_find_edge_cases();
    test_string_type_mismatch();
}