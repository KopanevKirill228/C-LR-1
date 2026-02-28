#include "fieldinfo.h"
#include <stdlib.h>
#include <stdio.h>

static void* int_copy(void* src) {
    if (src == NULL) return NULL;
    return src;
}

static void int_destroy(void* elem) {
    (void)elem;
}

static int int_compare(void* a, void* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return *(int*)a - *(int*)b;
}

static void int_print(FILE* out, void* elem) {
    if (elem != NULL) {
        fprintf(out, "%d", *(int*)elem);
    }
}

static FieldInfo* _int_field_info = NULL;

const FieldInfo* GetIntFieldInfo(void) {
    if (_int_field_info == NULL) {
        _int_field_info = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (_int_field_info != NULL) {
            _int_field_info->element_size = sizeof(int);
            _int_field_info->copy = int_copy;
            _int_field_info->destroy = int_destroy;
            _int_field_info->compare = int_compare;
            _int_field_info->print = int_print;
        }
    }
    return (const FieldInfo*)_int_field_info;
}