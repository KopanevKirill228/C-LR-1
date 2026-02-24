#include "my_string.h"
#include "Vector.h"
#include "fieldinfo.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


struct _string {
    Vector* data;
    char* c_string_cache;
    bool cache_valid;
};



static int char_compare_case_sensitive(void* a, void* b) {
    if (a == NULL || b == NULL) return 0;
    return *(char*)a - *(char*)b;
}

static int char_compare_case_insensitive(void* a, void* b) {
    if (a == NULL || b == NULL) return 0;
    return tolower(*(char*)a) - tolower(*(char*)b);
}

static void String_InvalidateCache(String* s) {
    if (s != NULL && s->c_string_cache != NULL) {
        free(s->c_string_cache);
        s->c_string_cache = NULL;
        s->cache_valid = false;
    }
}



String* String_Create(const char* c_str) {
    const FieldInfo* char_type = GetCharFieldInfo();
    if (char_type == NULL) {
        return NULL;
    }
    
    String* s = (String*)malloc(sizeof(String));
    if (s == NULL) {
        return NULL;
    }
    
    size_t len = (c_str != NULL) ? strlen(c_str) : 0;
    
    s->data = Vector_Create(char_type, len + 1);
    if (s->data == NULL) {
        free(s);
        return NULL;
    }
    
    s->c_string_cache = NULL;
    s->cache_valid = false;
    
    if (c_str != NULL) {
        for (size_t i = 0; i < len; i++) {
            char c = c_str[i];
            if (Vector_Push(s->data, &c) != 0) {
                String_Destroy(s);
                return NULL;
            }
        }
    }
    
    return s;
}

String* String_CreateEmpty(size_t capacity) {
    const FieldInfo* char_type = GetCharFieldInfo();
    if (char_type == NULL) {
        return NULL;
    }
    
    String* s = (String*)malloc(sizeof(String));
    if (s == NULL) {
        return NULL;
    }
    
    s->data = Vector_Create(char_type, capacity);
    if (s->data == NULL) {
        free(s);
        return NULL;
    }
    
    s->c_string_cache = NULL;
    s->cache_valid = false;
    
    return s;
}

void String_Destroy(String* s) {
    if (s == NULL) {
        return;
    }
    
    if (s->c_string_cache != NULL) {
        free(s->c_string_cache);
    }
    
    if (s->data != NULL) {
        Vector_Destroy(s->data);
    }
    
    free(s);
}



String* String_Concat(const String* s1, const String* s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }
    
    size_t len1 = String_Length(s1);
    size_t len2 = String_Length(s2);
    
    String* result = String_CreateEmpty(len1 + len2);
    if (result == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < len1; i++) {
        const char* c = (const char*)Vector_Get(s1->data, i);
        if (c != NULL) {
            Vector_Push(result->data, (void*)c);
        }
    }
    
    for (size_t i = 0; i < len2; i++) {
        const char* c = (const char*)Vector_Get(s2->data, i);
        if (c != NULL) {
            Vector_Push(result->data, (void*)c);
        }
    }
    
    return result;
}

String* String_Substring(const String* s, size_t start, size_t end) {
    if (s == NULL) {
        return NULL;
    }
    
    size_t len = String_Length(s);
    
    if (start > len || end > len || start > end) {
        return NULL;
    }
    
    String* result = String_CreateEmpty(end - start);
    if (result == NULL) {
        return NULL;
    }
    
    for (size_t i = start; i < end; i++) {
        const char* c = (const char*)Vector_Get(s->data, i);
        if (c != NULL) {
            Vector_Push(result->data, (void*)c);
        }
    }
    
    return result;
}

int* String_Find(const String* s, const String* pattern, bool case_sensitive, size_t* out_count) {
    if (s == NULL || pattern == NULL || out_count == NULL) {
        return NULL;
    }
    
    *out_count = 0;
    
    size_t text_len = String_Length(s);
    size_t pattern_len = String_Length(pattern);
    
    if (pattern_len == 0 || pattern_len > text_len) {
        return NULL;
    }
    
    int* indices = (int*)malloc(text_len * sizeof(int));
    if (indices == NULL) {
        return NULL;
    }
    
    int (*compare_func)(void*, void*) = case_sensitive 
        ? char_compare_case_sensitive 
        : char_compare_case_insensitive;
    
    for (size_t i = 0; i <= text_len - pattern_len; i++) {
        bool match = true;
        
        for (size_t j = 0; j < pattern_len; j++) {
            const char* text_char = (const char*)Vector_Get(s->data, i + j);
            const char* pattern_char = (const char*)Vector_Get(pattern->data, j);
            
            if (text_char == NULL || pattern_char == NULL) {
                match = false;
                break;
            }
            
            if (compare_func((void*)text_char, (void*)pattern_char) != 0) {
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

const char* String_ToCString(String* s) {
    if (s == NULL || s->data == NULL) {
        return NULL;
    }
    
    if (s->cache_valid && s->c_string_cache != NULL) {
        return s->c_string_cache;
    }
    
    String_InvalidateCache(s);
    
    size_t len = String_Length(s);
    
    s->c_string_cache = (char*)malloc(len + 1);
    if (s->c_string_cache == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < len; i++) {
        const char* c = (const char*)Vector_Get(s->data, i);
        s->c_string_cache[i] = (c != NULL) ? *c : '\0';
    }
    s->c_string_cache[len] = '\0';
    s->cache_valid = true;
    
    return s->c_string_cache;
}