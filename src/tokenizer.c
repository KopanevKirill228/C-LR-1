#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include "fieldinfo.h"


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
        return NULL;
    }
    
    size_t len = String_Length(s);
    if (len == 0) return NULL;
    
    // Создаём массив для хранения лексем
    DynamicArray* tokens = DynamicArray_Create(GetStringFieldInfo(), len);
    if (tokens == NULL) return NULL;
    
    size_t i = 0;
    size_t start = 0;
    int state = 0; // 0: вне лексемы, 1: в идентификаторе, 2: в разделителе
    
    while (i < len) {
        const char* c = (const char*)String_GetElement(s, i);
        if (c == NULL) {
            Tokenize_Destroy(tokens);
            return NULL;
        }
        
        char current = *c;
        bool is_space = is_whitespace(current);
        bool is_alnum_char = is_alnum(current);
        bool is_delim_char = is_delimiter(current);
        
        // Состояние 0: вне лексемы
        if (state == 0) {
            if (!is_space) {
                // Начинаем новую лексему
                start = i;
                if (is_alnum_char) {
                    state = 1; // идентификатор
                } else if (is_delim_char) {
                    state = 2; // разделитель
                } else {
                    // Неизвестный символ - пропускаем
                    i++;
                    continue;
                }
            }
        }
        // Состояние 1: в идентификаторе
        else if (state == 1) {
            if (!is_alnum_char || is_space || is_delim_char) {
                // Конец идентификатора
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
                state = 0;
                continue; // обрабатываем текущий символ заново
            }
        }
        // Состояние 2: в разделителе
        else if (state == 2) {
            // Разделитель всегда один символ
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
            state = 0;
            continue; // обрабатываем текущий символ заново
        }
        
        i++;
    }
    
    // Обрабатываем последнюю лексему
    if (state == 1 || state == 2) {
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
            fprintf(out, "'");
            String_Print(out, *token_ptr);
            fprintf(out, "'");
        }
    }
    fprintf(out, "]");
    fflush(out);
}

String* Tokenize_Get(const DynamicArray* tokens, size_t index) {
    if (tokens == NULL) return NULL;
    
    String** token_ptr = (String**)DynamicArray_Get(tokens, index);
    return (token_ptr != NULL) ? *token_ptr : NULL;
}

size_t Tokenize_Count(const DynamicArray* tokens) {
    return (tokens != NULL) ? DynamicArray_Size(tokens) : 0;
}

// Проверяет, является ли строка char-типом (поддерживает ли токенизацию)
bool Tokenize_Supported(const String* s) {
    return (s != NULL && String_GetType(s) == GetCharFieldInfo());
}