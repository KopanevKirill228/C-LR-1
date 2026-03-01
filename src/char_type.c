#include "fieldinfo.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static void* char_copy(void* src) {
    if (src == NULL) return NULL;

    return src;
}

static void char_destroy(void* elem) {
    (void)elem;

}

static int char_compare(void* a, void* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return *(char*)a - *(char*)b;
}

static int char_compare_ignore_case(void* a, void* b) {
    char ca = *(char*)a;
    char cb = *(char*)b;
    
    if (ca >= 'A' && ca <= 'Z') ca = ca + ('a' - 'A');
    if (cb >= 'A' && cb <= 'Z') cb = cb + ('a' - 'A');
    
    return ca - cb;  // без учёта регистра
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
            _char_field_info->destroy = char_destroy;
            _char_field_info->compare = char_compare;
            _char_field_info->compare_case_insensitive = char_compare_ignore_case;
            _char_field_info->print = char_print;
        }
    }
    return (const FieldInfo*)_char_field_info;
}