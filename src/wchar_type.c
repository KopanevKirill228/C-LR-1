// wchar_type.c
#include "fieldinfo.h"
#include <stdlib.h>
#include <stdio.h>

static void* wchar_copy(void* src) {
    if (src == NULL) return NULL;
    wchar_t* new_wchar = (wchar_t*)malloc(sizeof(wchar_t));
    if (new_wchar != NULL) {
        *new_wchar = *(wchar_t*)src;
    }
    return new_wchar;
}

static void wchar_destroy(void* elem) {
    if (elem != NULL) {
        free(elem);
    }
}

static int wchar_compare(void* a, void* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return *(wchar_t*)a - *(wchar_t*)b;
}

// ← Печать БЕЗ разделителей (как строка!)
static void wchar_print(FILE* out, void* elem) {
    if (elem != NULL) {
        fwprintf(out, L"%lc", *(wchar_t*)elem);  // ← Нет запятых!
    }
}

static FieldInfo* _wchar_field_info = NULL;

const FieldInfo* GetWCharFieldInfo(void) {
    if (_wchar_field_info == NULL) {
        _wchar_field_info = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (_wchar_field_info != NULL) {
            _wchar_field_info->element_size = sizeof(wchar_t);
            _wchar_field_info->copy = wchar_copy;
            _wchar_field_info->destroy = wchar_destroy;
            _wchar_field_info->compare = wchar_compare;
            _wchar_field_info->print = wchar_print;
        }
    }
    return (const FieldInfo*)_wchar_field_info;
}