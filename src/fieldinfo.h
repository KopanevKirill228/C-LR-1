#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>
#include <stdio.h>

typedef struct _field_info {
    size_t element_size;
    void* (*copy)(void*);
    void (*destroy)(void*);
    int (*compare)(void*, void*); // сравнение с учетом регистра
    int (*compare_case_insensitive)(void*, void*); // сравнение без учёта регистра
    void (*print)(FILE* out, void* elem);  // Печать элемента
} FieldInfo;

const FieldInfo* GetCharFieldInfo(void);
const FieldInfo* GetIntFieldInfo(void);
const FieldInfo* GetStringFieldInfo(void);

#endif