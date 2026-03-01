#include "test_macros.h"
#include "../src/my_string.h"
#include "../src/dynamic_array.h"
#include "../src/fieldinfo.h"

// Вспомогательная функция: создаёт String из DynamicArray
static String* create_string_from_da(DynamicArray* da) {
    if (da == NULL) return NULL;
    
    // Создаём пустую строку
    String* s = String_CreateEmpty(0, DynamicArray_GetType(da));
    if (s == NULL) return NULL;
    
    // Копируем элементы из DynamicArray в строку
    size_t size = DynamicArray_Size(da);
    for (size_t i = 0; i < size; i++) {
        const void* elem = DynamicArray_Get(da, i);
        if (elem == NULL) {
            String_Destroy(s);
            return NULL;
        }
        
        // Создаём временную строку из одного элемента и конкатенируем
        String* temp = String_Create(elem, 1, DynamicArray_GetType(da));
        if (temp == NULL) {
            String_Destroy(s);
            return NULL;
        }
        
        String* new_s = String_Concat(s, temp);
        String_Destroy(s);
        String_Destroy(temp);
        
        if (new_s == NULL) return NULL;
        s = new_s;
    }
    
    return s;
}

// Вспомогательная функция: создаёт DynamicArray из C-строки (только для тестовых данных)
static DynamicArray* create_da_from_cstring(const char* str) {
    if (str == NULL) return NULL;
    
    DynamicArray* da = DynamicArray_Create(GetCharFieldInfo(), strlen(str));
    if (da == NULL) return NULL;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (DynamicArray_Push(da, (void*)&str[i]) != 0) {
            DynamicArray_Destroy(da);
            return NULL;
        }
    }
    return da;
}

// Вспомогательная функция: создаёт DynamicArray из int массива (только для тестовых данных)
static DynamicArray* create_da_from_intarray(const int* arr, size_t count) {
    if (arr == NULL || count == 0) return NULL;
    
    DynamicArray* da = DynamicArray_Create(GetIntFieldInfo(), count);
    if (da == NULL) return NULL;
    
    for (size_t i = 0; i < count; i++) {
        if (DynamicArray_Push(da, (void*)&arr[i]) != 0) {
            DynamicArray_Destroy(da);
            return NULL;
        }
    }
    return da;
}

// Вспомогательная функция: проверить содержимое строки с C-строкой
static int string_equals_cstring(const String* s, const char* expected) {
    if (s == NULL || expected == NULL) return 0;
    
    size_t len = String_Length(s);
    if (len != strlen(expected)) return 0;
    
    for (size_t i = 0; i < len; i++) {
        const char* c = (const char*)String_GetElement(s, i);
        if (c == NULL || *c != expected[i]) return 0;
    }
    return 1;
}

// Вспомогательная функция: проверить содержимое строки с int массивом
static int string_equals_intarray(const String* s, const int* expected, size_t count) {
    if (s == NULL || expected == NULL) return 0;
    
    size_t len = String_Length(s);
    if (len != count) return 0;
    
    for (size_t i = 0; i < len; i++) {
        const int* val = (const int*)String_GetElement(s, i);
        if (val == NULL || *val != expected[i]) return 0;
    }
    return 1;
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
    
    // Граничный случай 5: Создание из NULL данных
    s = String_Create(NULL, 5, GetCharFieldInfo());
    TEST(s != NULL, "Create with NULL data works (creates empty)");
    TEST(String_Length(s) == 0, "Length = 0");
    String_Destroy(s);
}

void test_string_concat_edge_cases(void) {
    printf("\n=== Concat Edge Cases ===\n");
    
    // Создаём данные через DynamicArray
    DynamicArray* da1 = create_da_from_cstring("Hello");
    DynamicArray* da2 = create_da_from_cstring(" World");
    DynamicArray* da_empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    
    String* s1 = create_string_from_da(da1);
    String* s2 = create_string_from_da(da2);
    String* s_empty = create_string_from_da(da_empty);
    
    TEST(s1 != NULL && s2 != NULL && s_empty != NULL, "Test strings created");
    
    // Граничный случай 1: Конкатенация с пустой строкой
    String* result = String_Concat(s1, s_empty);
    TEST(result != NULL, "Concat with empty works");
    TEST(String_Length(result) == 5, "Length = 5 (same as s1)");
    TEST(string_equals_cstring(result, "Hello"), "Result equals original when concat with empty");
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
    DynamicArray* da_long = DynamicArray_Create(GetCharFieldInfo(), 1000);
    char c = 'a';
    for(int i = 0; i < 1000; i++) {
        DynamicArray_Push(da_long, &c);
    }
    String* s_long = create_string_from_da(da_long);
    
    result = String_Concat(s_long, s_long);
    TEST(result != NULL, "Concat two long strings");
    TEST(String_Length(result) == 2000, "Length = 2000");
    String_Destroy(result);
    
    String_Destroy(s_long);
    DynamicArray_Destroy(da_long);
    
    // Граничный случай 5: Конкатенация строк разного типа
    DynamicArray* da_int = create_da_from_intarray((int[]){1,2,3}, 3);
    String* s_int = create_string_from_da(da_int);
    
    TEST(String_Concat(s1, s_int) == NULL, "Concat different types fails");
    
    String_Destroy(s_int);
    DynamicArray_Destroy(da_int);
    
    String_Destroy(s1);
    String_Destroy(s2);
    String_Destroy(s_empty);
    DynamicArray_Destroy(da1);
    DynamicArray_Destroy(da2);
    DynamicArray_Destroy(da_empty);
}

void test_string_substring_edge_cases(void) {
    printf("\n=== Substring Edge Cases ===\n");
    
    DynamicArray* da = create_da_from_cstring("Hello World");
    String* s = create_string_from_da(da);
    TEST(s != NULL, "Test string created");
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
    TEST(string_equals_cstring(sub, "World"), "Substring [6,end) = 'World'");
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
    TEST(String_Substring(s, -1, 5) == NULL, "negative start returns NULL");
    
    // Граничный случай 6: Substring от NULL
    TEST(String_Substring(NULL, 0, 5) == NULL, "Substring from NULL returns NULL");
    
    String_Destroy(s);
    DynamicArray_Destroy(da);
}

void test_string_find_edge_cases(void) {
    printf("\n=== Find Edge Cases ===\n");
    
    // Создаём строку "abababa" через DynamicArray
    DynamicArray* da_text = DynamicArray_Create(GetCharFieldInfo(), 7);
    char chars[] = {'a','b','a','b','a','b','a'};
    for(int i = 0; i < 7; i++) {
        DynamicArray_Push(da_text, &chars[i]);
    }
    String* text = create_string_from_da(da_text);
    TEST(text != NULL, "Test text created");
    
    // Граничный случай 1: Пустой паттерн
    DynamicArray* da_empty = DynamicArray_Create(GetCharFieldInfo(), 0);
    String* empty_pat = create_string_from_da(da_empty);
    
    size_t count;
    int* indices = String_Find(text, empty_pat, true, &count);
    TEST(indices == NULL && count == 0, "Empty pattern not found");
    
    // Граничный случай 2: Паттерн длиннее текста
    DynamicArray* da_long = create_da_from_cstring("abababaabababa");
    String* long_pat = create_string_from_da(da_long);
    
    indices = String_Find(text, long_pat, true, &count);
    TEST(indices == NULL && count == 0, "Pattern longer than text not found");
    String_Destroy(long_pat);
    DynamicArray_Destroy(da_long);
    
    // Граничный случай 3: Паттерн в самом начале
    DynamicArray* da_start = create_da_from_cstring("aba");
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
    
    // Граничный случай 4: Паттерн из одного символа
    DynamicArray* da_one = create_da_from_cstring("a");
    String* one_pat = create_string_from_da(da_one);
    
    indices = String_Find(text, one_pat, true, &count);
    TEST(count == 4, "Single char 'a' appears 4 times");
    if(indices) free(indices);
    String_Destroy(one_pat);
    DynamicArray_Destroy(da_one);
    
    // Граничный случай 5: Поиск с NULL параметрами
    TEST(String_Find(NULL, text, true, &count) == NULL, "Find with NULL text");
    TEST(String_Find(text, NULL, true, &count) == NULL, "Find with NULL pattern");
    TEST(String_Find(text, text, true, NULL) == NULL, "Find with NULL out_count");
    
    
    String_Destroy(text);
    String_Destroy(empty_pat);
    DynamicArray_Destroy(da_text);
    DynamicArray_Destroy(da_empty);
}

void test_string_int_operations(void) {
    printf("\n=== Int String Operations ===\n");
    
    // Создаём int строку через DynamicArray
    int nums1[] = {1, 2, 3, 4, 5};
    DynamicArray* da1 = create_da_from_intarray(nums1, 5);
    String* s1 = create_string_from_da(da1);
    TEST(s1 != NULL, "Int string created");
    
    // Проверка длины
    TEST(String_Length(s1) == 5, "Int string length = 5");
    
    // Проверка элементов
    int correct = 1;
    for (int i = 0; i < 5; i++) {
        const int* val = (const int*)String_GetElement(s1, i);
        if (val == NULL || *val != nums1[i]) {
            correct = 0;
            break;
        }
    }
    TEST(correct, "Int string elements correct");
    
    // Конкатенация int строк
    int nums2[] = {6, 7, 8};
    DynamicArray* da2 = create_da_from_intarray(nums2, 3);
    String* s2 = create_string_from_da(da2);
    
    String* result = String_Concat(s1, s2);
    TEST(result != NULL, "Concat int strings works");
    TEST(String_Length(result) == 8, "Concatenated length = 8");
    
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    TEST(string_equals_intarray(result, expected, 8), "Concatenated content correct");
    
    String_Destroy(result);
    String_Destroy(s2);
    DynamicArray_Destroy(da2);
    
    // Подстрока int строки
    String* sub = String_Substring(s1, 1, 4);
    TEST(sub != NULL, "Int substring works");
    TEST(String_Length(sub) == 3, "Substring length = 3");
    
    int expected_sub[] = {2, 3, 4};
    TEST(string_equals_intarray(sub, expected_sub, 3), "Substring content correct");
    String_Destroy(sub);
    
    // Поиск в int строке
    int pattern_nums[] = {3, 4};
    DynamicArray* da_pat = create_da_from_intarray(pattern_nums, 2);
    String* pattern = create_string_from_da(da_pat);
    
    size_t count;
    int* indices = String_Find(s1, pattern, true, &count);
    TEST(count == 1, "Pattern found once");
    if (indices) {
        TEST(indices[0] == 2, "Pattern at position 2");
        free(indices);
    }
    
    String_Destroy(pattern);
    DynamicArray_Destroy(da_pat);
    
    String_Destroy(s1);
    DynamicArray_Destroy(da1);
}

void run_string_tests(void) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   STRING TESTS — Полное тестирование   ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    test_string_create_edge_cases();
    test_string_concat_edge_cases();
    test_string_substring_edge_cases();
    test_string_find_edge_cases();
    test_string_int_operations();
}