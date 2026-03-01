#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>


//  Проверяет, является ли символ разделителем/оператором
static int is_delimiter(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || 
            c == '=' || c == '(' || c == ')' || c == ',' ||
            c == ';');
}


// Проверяет, является ли символ пробельным
static int is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

// Проверяет, является ли символ буквой или цифрой
static int is_alnum(char c) {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9');
}



DynamicArray* Tokenize(const String* s) {
    // Проверка: только для char строк
    if (!Tokenize_Supported(s)) {
        return NULL; //иначе нельзя
    }
    
    size_t len = String_Length(s);
    if (len == 0) return NULL;
    
    // Создаём массив для хранения лексем
    DynamicArray* tokens = DynamicArray_Create(GetCharFieldInfo(), 10);
    if (tokens == NULL) return NULL;
    
    size_t i = 0;
    while (i < len) {
        // Пропускаем пробельные символы
        while (i < len) {
            const char* c = (const char*)String_GetElement(s, i);
            if (c == NULL) {
                Tokenize_Destroy(tokens);
                return NULL;
            }
            if (!is_whitespace(*c)) break;
            i++;
        }
        
        if (i >= len) break;
        
        const char* current = (const char*)String_GetElement(s, i);
        if (current == NULL) {
            Tokenize_Destroy(tokens);
            return NULL;
        }
        
        size_t start = i;
        
        // Случай 1: Идентификатор (буквы и цифры)
        if (is_alnum(*current)) {
            // Собираем последовательность букв/цифр
            while (i < len) {
                const char* c = (const char*)String_GetElement(s, i);
                if (c == NULL || !is_alnum(*c)) break;
                i++;
            }
            
            // Создаём лексему-идентификатор
            String* token = String_Substring(s, start, i);
            if (token == NULL) {
                Tokenize_Destroy(tokens);
                return NULL;
            }
            
            // Сохраняем указатель на лексему в массиве
            if (DynamicArray_Push(tokens, &token) != 0) {
                String_Destroy(token);
                Tokenize_Destroy(tokens);
                return NULL;
            }
        }
        // Случай 2: Разделитель или оператор (одиночный символ)
        else if (is_delimiter(*current)) {
            i++; // Берём один символ
            
            String* token = String_Substring(s, start, i);
            if (token == NULL) {
                Tokenize_Destroy(tokens);
                return NULL;
            }
            
            if (DynamicArray_Push(tokens, &token) != 0) {
                String_Destroy(token);
                Tokenize_Destroy(tokens);
                return NULL;
            }
        }
        // Случай 3: Неизвестный символ (пропускаем)
        else {
            i++;
        }
    }
    
    return tokens;
}

void Tokenize_Destroy(DynamicArray* tokens) {
    if (tokens == NULL) return;
    
    // Освобождаем каждую лексему
    size_t size = DynamicArray_Size(tokens);
    for (size_t i = 0; i < size; i++) {
        String** token_ptr = (String**)DynamicArray_Get(tokens, i);
        if (token_ptr != NULL && *token_ptr != NULL) {
            String_Destroy(*token_ptr);
        }
    }
    
    DynamicArray_Destroy(tokens);
}

void Tokenize_Print(const DynamicArray* tokens, FILE* out) {
    if (tokens == NULL) {
        fprintf(out, "NULL");
        return;
    }
    
    fprintf(out, "[");
    size_t size = DynamicArray_Size(tokens);
    for (size_t i = 0; i < size; i++) {
        String** token_ptr = (String**)DynamicArray_Get(tokens, i);
        if (token_ptr != NULL && *token_ptr != NULL) {
            if (i > 0) fprintf(out, ", ");
            String_Print(out, *token_ptr);
        }
    }
    fprintf(out, "]");
}

String* Tokenize_Get(const DynamicArray* tokens, size_t index) {
    if (tokens == NULL) return NULL;
    
    String** token_ptr = (String**)DynamicArray_Get(tokens, index);
    return (token_ptr != NULL) ? *token_ptr : NULL;
}

size_t Tokenize_Count(const DynamicArray* tokens) {
    return (tokens != NULL) ? DynamicArray_Size(tokens) : 0;
}

bool Tokenize_Supported(const String* s) {
    return (s != NULL && String_GetType(s) == GetCharFieldInfo());
}