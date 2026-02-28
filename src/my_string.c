// my_string.c
#include "my_string.h"
#include "Vector.h"
#include "fieldinfo.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct _string {
    Vector* data;
    const FieldInfo* type_info;  // ← Храним тип для полиморфизма
};

String* String_Create(const void* data, size_t element_count, const FieldInfo* type_info) {
    if (type_info == NULL) return NULL;
    
    String* s = (String*)malloc(sizeof(String));
    if (s == NULL) return NULL;
    
    s->type_info = type_info;
    s->data = Vector_Create(type_info, element_count);
    if (s->data == NULL) {
        free(s);
        return NULL;
    }
    
    if (data != NULL && element_count > 0) {
        for (size_t i = 0; i < element_count; i++) {
            const unsigned char* byte_ptr = (const unsigned char*)data;
            const void* elem = byte_ptr + i * type_info->element_size;
            if (Vector_Push(s->data, (void*)elem) != 0) {
                String_Destroy(s);
                return NULL;
            }
        }
    }
    
    return s;
}

String* String_CreateEmpty(size_t capacity, const FieldInfo* type_info) {
    return String_Create(NULL, 0, type_info);
}

void String_Destroy(String* s) {
    if (s == NULL) return;
    if (s->data != NULL) Vector_Destroy(s->data);
    free(s);
}

String* String_Concat(const String* s1, const String* s2) {
    if (s1 == NULL || s2 == NULL || !String_SameType(s1, s2)) return NULL;
    
    size_t len1 = String_Length(s1);
    size_t len2 = String_Length(s2);
    
    String* result = String_CreateEmpty(len1 + len2, s1->type_info);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len1; i++) {
        const void* elem = String_GetElement(s1, i);
        if (elem != NULL) Vector_Push(result->data, (void*)elem);
    }
    for (size_t i = 0; i < len2; i++) {
        const void* elem = String_GetElement(s2, i);
        if (elem != NULL) Vector_Push(result->data, (void*)elem);
    }
    
    return result;
}

String* String_Substring(const String* s, size_t start, size_t end) {
    if (s == NULL) return NULL;
    
    size_t len = String_Length(s);
    if (start > len || end > len || start > end) return NULL;
    
    String* result = String_CreateEmpty(end - start, s->type_info);
    if (result == NULL) return NULL;
    
    for (size_t i = start; i < end; i++) {
        const void* elem = String_GetElement(s, i);
        if (elem != NULL) Vector_Push(result->data, (void*)elem);
    }
    
    return result;
}

int* String_Find(const String* s, const String* pattern, bool match_case, size_t* out_count) {
    if (s == NULL || pattern == NULL || out_count == NULL || !String_SameType(s, pattern)) {
        return NULL;
    }
    
    *out_count = 0;
    size_t text_len = String_Length(s);
    size_t pattern_len = String_Length(pattern);
    
    if (pattern_len == 0 || pattern_len > text_len) return NULL;
    
    int* indices = (int*)malloc(text_len * sizeof(int));
    if (indices == NULL) return NULL;
    
    int (*compare_func)(void*, void*) = s->type_info->compare;
    
    for (size_t i = 0; i <= text_len - pattern_len; i++) {
        bool match = true;
        for (size_t j = 0; j < pattern_len; j++) {
            const void* text_elem = String_GetElement(s, i + j);
            const void* pattern_elem = String_GetElement(pattern, j);
            
            if (text_elem == NULL || pattern_elem == NULL) {
                match = false;
                break;
            }
            
            if (compare_func((void*)text_elem, (void*)pattern_elem) != 0) {
                match = false;
                break;
            }
        }
        
        if (match) {
            indices[*out_count] = (int)i;
            (*out_count)++;
        }
    }
    
    if (*out_count == 0) {
        free(indices);
        return NULL;
    }
    
    return indices;
}

size_t String_Length(const String* s) {
    return (s != NULL && s->data != NULL) ? Vector_Size(s->data) : 0;
}

const void* String_GetElement(const String* s, size_t index) {
    if (s == NULL || s->data == NULL) return NULL;
    return Vector_Get(s->data, index);
}

// ← УНИВЕРСАЛЬНАЯ ПЕЧАТЬ (как СТРОКА, не массив!)
void String_Print(FILE* out, const String* s) {
    if (s == NULL || s->data == NULL || s->type_info == NULL) return;
    
    if (s->type_info->print == NULL) {
        fprintf(out, "[no print]");
        return;
    }
    
    // ← Нет скобок и запятых — вывод как строка!
    for (size_t i = 0; i < String_Length(s); i++) {
        const void* elem = String_GetElement(s, i);
        s->type_info->print(out, (void*)elem);  // ← Полиморфный вызов
    }
}

void String_PrintLine(const String* s) {
    String_Print(stdout, s);
    printf("\n");
}

const FieldInfo* String_GetType(const String* s) {
    return (s != NULL) ? s->type_info : NULL;
}

bool String_SameType(const String* s1, const String* s2) {
    if (s1 == NULL || s2 == NULL) return false;
    return (s1->type_info == s2->type_info);
}