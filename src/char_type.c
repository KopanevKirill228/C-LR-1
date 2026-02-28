// char_type.c
#include "fieldinfo.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static void* char_copy(void* src) {
    if (src == NULL) return NULL;
    char* new_char = (char*)malloc(sizeof(char));
    if (new_char != NULL) {
        *new_char = *(char*)src;
    }
    return new_char;
}

// ПРАВИЛЬНО: для простых типов destroy ничего не делает
static void char_destroy(void* elem) {
    (void)elem;  // Подавляем warning о неиспользуемом параметре
    // Не освобождаем elem, так как он указывает внутрь массива Vector
    // Вся память будет освобождена при free(vec->data)
}

static int char_compare(void* a, void* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return *(char*)a - *(char*)b;
}

static void char_print(FILE* out, void* elem) {
    if (elem != NULL) {
        fprintf(out, "%c", *(char*)elem);
    }
}

static FieldInfo* _char_field_info = NULL;

const FieldInfo* GetCharFieldInfo(void) {
    if (_char_field_info == NULL) {
        _char_field_info = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (_char_field_info != NULL) {
            _char_field_info->element_size = sizeof(char);
            _char_field_info->copy = char_copy;
            _char_field_info->destroy = char_destroy;  // ← теперь правильно
            _char_field_info->compare = char_compare;
            _char_field_info->print = char_print;
        }
    }
    return (const FieldInfo*)_char_field_info;
}