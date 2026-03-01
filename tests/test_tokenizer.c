#include "test_macros.h"
#include "../src/tokenizer.h"
#include "../src/my_string.h"
#include "../src/dynamic_array.h"
#include "../src/fieldinfo.h"
#include <stdio.h>

// Вспомогательная функция для создания String из C-строки
static String* create_test_string(const char* str) {
    if (str == NULL) return NULL;
    return String_Create(str, strlen(str), GetCharFieldInfo());
}

// Вспомогательная функция для проверки лексемы
static int check_token(const DynamicArray* tokens, size_t index, const char* expected) {
    String* token = Tokenize_Get(tokens, index);
    if (token == NULL) return 0;
    
    size_t token_len = String_Length(token);
    if (token_len != strlen(expected)) return 0;
    
    for (size_t i = 0; i < token_len; i++) {
        const char* c = (const char*)String_GetElement(token, i);
        if (c == NULL || *c != expected[i]) return 0;
    }
    return 1;
}

// Тест 1: Простое выражение без пробелов
void test_tokenizer_simple(void) {
    TEST_GROUP("Tokenizer: простые выражения");
    
    String* s = create_test_string("ABC+D");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize 'ABC+D' работает");
    TEST(Tokenize_Count(tokens) == 3, "Должно быть 3 лексемы");
    
    if (tokens && Tokenize_Count(tokens) == 3) {
        TEST(check_token(tokens, 0, "ABC"), "Первая лексема 'ABC'");
        TEST(check_token(tokens, 1, "+"), "Вторая лексема '+'");
        TEST(check_token(tokens, 2, "D"), "Третья лексема 'D'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 2: Выражение с запятой
void test_tokenizer_with_comma(void) {
    TEST_GROUP("Tokenizer: выражение с запятой");
    
    String* s = create_test_string("ABC,+D");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize 'ABC,+D' работает");
    TEST(Tokenize_Count(tokens) == 4, "Должно быть 4 лексемы");
    
    if (tokens && Tokenize_Count(tokens) == 4) {
        TEST(check_token(tokens, 0, "ABC"), "Первая лексема 'ABC'");
        TEST(check_token(tokens, 1, ","), "Вторая лексема ','");
        TEST(check_token(tokens, 2, "+"), "Третья лексема '+'");
        TEST(check_token(tokens, 3, "D"), "Четвёртая лексема 'D'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 3: Выражение с пробелами
void test_tokenizer_with_spaces(void) {
    TEST_GROUP("Tokenizer: выражение с пробелами");
    
    String* s = create_test_string("a + b * c");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize 'a + b * c' работает");
    TEST(Tokenize_Count(tokens) == 5, "Должно быть 5 лексем");
    
    if (tokens && Tokenize_Count(tokens) == 5) {
        TEST(check_token(tokens, 0, "a"), "Лексема 0: 'a'");
        TEST(check_token(tokens, 1, "+"), "Лексема 1: '+'");
        TEST(check_token(tokens, 2, "b"), "Лексема 2: 'b'");
        TEST(check_token(tokens, 3, "*"), "Лексема 3: '*'");
        TEST(check_token(tokens, 4, "c"), "Лексема 4: 'c'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 4: Выражение с функцией
void test_tokenizer_function(void) {
    TEST_GROUP("Tokenizer: вызов функции");
    
    String* s = create_test_string("f(int)");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize 'f(int)' работает");
    TEST(Tokenize_Count(tokens) == 4, "Должно быть 4 лексемы");
    
    if (tokens && Tokenize_Count(tokens) == 4) {
        TEST(check_token(tokens, 0, "f"), "Лексема 0: 'f'");
        TEST(check_token(tokens, 1, "("), "Лексема 1: '('");
        TEST(check_token(tokens, 2, "int"), "Лексема 2: 'int'");
        TEST(check_token(tokens, 3, ")"), "Лексема 3: ')'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 5: Сложное выражение
void test_tokenizer_complex(void) {
    TEST_GROUP("Tokenizer: сложное выражение");
    
    String* s = create_test_string("x = y + 10;");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize 'x = y + 10 +' работает");
    TEST(Tokenize_Count(tokens) == 6, "Должно быть 6 лексем");
    
    if (tokens && Tokenize_Count(tokens) == 5) {
        TEST(check_token(tokens, 0, "x"), "Лексема 0: 'x'");
        TEST(check_token(tokens, 1, "="), "Лексема 1: '='");
        TEST(check_token(tokens, 2, "y"), "Лексема 2: 'y'");
        TEST(check_token(tokens, 3, "+"), "Лексема 3: '+'");
        TEST(check_token(tokens, 4, "10"), "Лексема 4: '10'");
        TEST(check_token(tokens, 5, "+"), "Лексема 4: '+'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 6: Много пробелов
void test_tokenizer_many_spaces(void) {
    TEST_GROUP("Tokenizer: много пробелов");
    
    String* s = create_test_string("  a  +  b  *  c  ");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize с пробелами работает");
    TEST(Tokenize_Count(tokens) == 5, "Пробелы игнорируются, 5 лексем");
    
    if (tokens && Tokenize_Count(tokens) == 5) {
        TEST(check_token(tokens, 0, "a"), "Первая 'a'");
        TEST(check_token(tokens, 1, "+"), "Вторая '+'");
        TEST(check_token(tokens, 2, "b"), "Третья 'b'");
        TEST(check_token(tokens, 3, "*"), "Четвёртая '*'");
        TEST(check_token(tokens, 4, "c"), "Пятая 'c'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 7: Числа
void test_tokenizer_numbers(void) {
    TEST_GROUP("Tokenizer: числа");
    
    String* s = create_test_string("123 + 456");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Tokenize с числами работает");
    TEST(Tokenize_Count(tokens) == 3, "Должно быть 3 лексемы");
    
    if (tokens && Tokenize_Count(tokens) == 3) {
        TEST(check_token(tokens, 0, "123"), "Первая '123'");
        TEST(check_token(tokens, 1, "+"), "Вторая '+'");
        TEST(check_token(tokens, 2, "456"), "Третья '456'");
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 8: Пустая строка
void test_tokenizer_empty(void) {
    TEST_GROUP("Tokenizer: пустая строка");
    
    String* s = String_CreateEmpty(0, GetCharFieldInfo());
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens == NULL, "Пустая строка возвращает NULL");
    
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 9: Неподдерживаемый тип (int)
void test_tokenizer_unsupported(void) {
    TEST_GROUP("Tokenizer: неподдерживаемый тип");
    
    int data[] = {1, 2, 3};
    String* s = String_Create(data, 3, GetIntFieldInfo());
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens == NULL, "int строка возвращает NULL");
    
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Тест 10: Только разделители
void test_tokenizer_only_delimiters(void) {
    TEST_GROUP("Tokenizer: только разделители");
    
    String* s = create_test_string("+ - * / = ( ) , ;");
    DynamicArray* tokens = Tokenize(s);
    
    TEST(tokens != NULL, "Только разделители работают");
    
    if (tokens) {
        TEST(Tokenize_Count(tokens) == 9, "Должно быть 9 разделителей");
        
        // Проверим первые несколько
        if (Tokenize_Count(tokens) >= 4) {
            TEST(check_token(tokens, 0, "+"), "Первый '+'");
            TEST(check_token(tokens, 1, "-"), "Второй '-'");
            TEST(check_token(tokens, 2, "*"), "Третий '*'");
            TEST(check_token(tokens, 3, "/"), "Четвёртый '/'");
        }
    }
    
    Tokenize_Destroy(tokens);
    String_Destroy(s);
    
    TEST_GROUP_END();
}

// Главная функция запуска тестов токенайзера
void run_tokenizer_tests(void) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   TOKENIZER TESTS — Разбиение на лексемы║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    test_tokenizer_simple();
    test_tokenizer_with_comma();
    test_tokenizer_with_spaces();
    test_tokenizer_function();
    test_tokenizer_complex();
    test_tokenizer_many_spaces();
    test_tokenizer_numbers();
    test_tokenizer_empty();
    test_tokenizer_unsupported();
    test_tokenizer_only_delimiters();
}