#include "fieldinfo.h"
#include <stdlib.h>
#include <stdio.h>
#include "my_string.h"


static void* string_copy(void* src) {
    if (src == NULL) return NULL;
    return src;
}


static void string_destroy(void* elem) {
    (void)elem;
}

static int string_compare(void* a, void* b) {
    (void)a;
    (void)b;
    return 0;  // Заглушка, если compare всё же требуется. Ведь нельзя сравнивать строки
}

// Печать: печатаем строку в кавычках
static void string_print(FILE* out, void* elem) {
    if (elem == NULL) {
        fprintf(out, "NULL");
        return;
    }
    
    String* str = *(String**)elem;
    if (str == NULL) {
        fprintf(out, "NULL");
        return;
    }
    
    fprintf(out, "\"");
    String_Print(out, str);
    fprintf(out, "\"");
}

static FieldInfo* _string_field_info = NULL;

const FieldInfo* GetStringFieldInfo(void) {
    if (_string_field_info == NULL) {
        _string_field_info = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (_string_field_info != NULL) {
            _string_field_info->element_size = sizeof(String*);
            _string_field_info->copy = string_copy;
            _string_field_info->destroy = string_destroy;
            _string_field_info->compare = string_compare;
            _string_field_info->print = string_print;
        }
    }
    return (const FieldInfo*)_string_field_info;
}