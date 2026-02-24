#include "fieldinfo.h"
#include <stdlib.h>
#include <ctype.h>

/* ============================================================================
 * Внутренние функции для работы с char
 * ========================================================================== */

static void* char_copy(void* src) {
    if (src == NULL) return NULL;
    char* new_char = (char*)malloc(sizeof(char));
    if (new_char != NULL) {
        *new_char = *(char*)src;
    }
    return new_char;
}

static void char_destroy(void* elem) {
    if (elem != NULL) {
        free(elem);
    }
}

static int char_compare(void* a, void* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return *(char*)a - *(char*)b;
}

/* ============================================================================
 * Ленивая инициализация (Singleton)
 * ========================================================================== */

static FieldInfo* _char_field_info = NULL;

const FieldInfo* GetCharFieldInfo(void) {
    if (_char_field_info == NULL) {
        _char_field_info = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (_char_field_info != NULL) {
            _char_field_info->element_size = sizeof(char);
            _char_field_info->copy = char_copy;
            _char_field_info->destroy = char_destroy;
            _char_field_info->compare = char_compare;
        }
    }
    return (const FieldInfo*)_char_field_info;
}